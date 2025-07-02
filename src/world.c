#include "world.h"

// Could use sun position and portals to simulate a round earth???

static vec3 getPlayerPosWithinChunk(vec3 position);
static vec3 getWorldCoordFromChunkCoord(vec3 chunkCoord, int x, int y, int z);
static vec3 getChunkCoordFromWorldCoord(vec3 position);

World createWorld(void) {
    World world;
    world.dayNightCircle= WORLD_START_TIME * 2*PI;
    world.dayLength= DEFAULT_DAYLIGHT_CYCLE;
    world.timeElapsed= 0;

    // Load sun cube vertices
    glGenVertexArrays(1, &world.sunVAO);
    glBindVertexArray(world.sunVAO);

    glGenBuffers(1, &world.sunVBO);
    glBindBuffer(GL_ARRAY_BUFFER, world.sunVBO);

    glGenBuffers(1, &world.sunIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world.sunIBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 12*sizeof(float), (void*) (sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*) (sizeof(float)*6));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, 12*sizeof(float), (void*) (sizeof(float)*10));

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glActiveTexture(WORLD_TEXTURE);

    // Load blockAtlas texture
    stbi_set_flip_vertically_on_load(true);
    world.blockAtlas= stbi_load("textures/BlockAtlas.png", &world.blockAtlasX, &world.blockAtlasY, &world.blockAtlasChannels, 0);
    glGenTextures(1, &world.glBlockAtlas);
    glBindTexture(GL_TEXTURE_2D, world.glBlockAtlas);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, world.blockAtlasX, world.blockAtlasY, 0, GL_RGBA, GL_UNSIGNED_BYTE, world.blockAtlas);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Load starMap cubemap texture
    world.northStarMap= stbi_load("textures/NorthStarMap.png", &world.starMapX, &world.starMapY, &world.starMapChannels, 0);
    world.starMap= stbi_load("textures/StarMap.png", &world.starMapX, &world.starMapY, &world.starMapChannels, 0);
    glGenTextures(1, &world.glStarMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, world.glStarMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (int i= 0; i < 6; i++) {
        if (GL_TEXTURE_CUBE_MAP_POSITIVE_X + i == GL_TEXTURE_CUBE_MAP_POSITIVE_Y) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, world.starMapX, world.starMapY, 0, GL_RGBA, GL_UNSIGNED_BYTE, world.northStarMap);
        } else {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, world.starMapX, world.starMapY, 0, GL_RGBA, GL_UNSIGNED_BYTE, world.starMap);
        }
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // Generate chunks
    world.playerWorldOffsetX= WORLD_SIZE/2;
    world.playerWorldOffsetY= WORLD_HEIGHT/2;
    world.playerWorldOffsetZ= WORLD_SIZE/2;
    for (int i= 0; i < WORLD_SIZE; i++) {
        for (int j= WORLD_HEIGHT-1; j >= 0; j--) {
            for (int k= 0; k < WORLD_SIZE; k++) {
                // Do the most basic possible interesting terrain generation
                int blockTemplate[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM];
                for (int x= 0; x < CHUNK_DIM; x++) {
                    for (int y= CHUNK_DIM-1; y >= 0; y--) {
                        for (int z= 0; z < CHUNK_DIM; z++) {
                            vec3 worldCoord= getWorldCoordFromChunkCoord((vec3) {{i, j-2, k}}, x, y, z);
                            // Add dirt
                            blockTemplate[x][y][z]= worldCoord.y < 5 * sin(RAD(7*worldCoord.x)) * cos(RAD(7*worldCoord.z)) ? bt_Dirt : 0;
                            // Add stone
                            if (worldCoord.y < 5 * sin(RAD(7*worldCoord.x)) * cos(RAD(7*worldCoord.z)) - 5) {
                                blockTemplate[x][y][z]= bt_Stone;
                            }
                            // Convert dirt to grass under certain conditions
                            if ((y != CHUNK_DIM-1 && blockTemplate[x][y][z] == bt_Dirt && blockTemplate[x][y+1][z] == 0) ||
                                (y == CHUNK_DIM-1 && blockTemplate[x][y][z] == bt_Dirt && j == WORLD_HEIGHT-1) ||
                                (y == CHUNK_DIM-1 && blockTemplate[x][y][z] == bt_Dirt && j != WORLD_HEIGHT-1 && world.chunkArray[i][j+1][k].blockData[CHUNK_DIM*CHUNK_DIM*x + z] == 0)) {
                                blockTemplate[x][y][z]= bt_Grass;
                            }
                        }
                    }
                }
                // Load chunk
                world.chunkArray[i][j][k]= createChunkMesh((vec3) {{i, j-2, k}}, blockTemplate);
            }
        }
    }
    return world;
}

void world_renderPrepare(World* world, double deltaTime, vec3 playerPos) {
    // Prepare the sun
    world->dayNightCircle += deltaTime*(2*PI/world->dayLength);
    world->timeElapsed += deltaTime;
    memcpy(world->sunVertices, cubeVertices, sizeof(cubeVertices));
    // 1. Rotate at an arbitrary speed around an arbitrary axis to add life to the sun cube
    // 2. Translate relative to the player (to give the sun height and indicate south direction)
    // 3. Rotate the sun around the player ( The earth is flat ;) )
    mat4 sunTranspose= linAlg_Rotate(linAlg_Translate(linAlg_Rotate(linAlg_Scale(linAlg_Id(), 32, 32, 32), (vec3) {{1.0, 1.0, 1.0}}, world->dayNightCircle*7), playerPos.x, playerPos.y+500, playerPos.z+250), (vec3) {{0.0, -0.5, 1.0}}, world->dayNightCircle);
    world->sunPosition= vec4_to_vec3(mat4_X_vec4(sunTranspose, (vec4) {{0.0, 0.0, 0.0, 1.0}}));
    for (int i= 0; i < 24; i++) {
        world->sunVertices[i].position= vec4_to_vec3(mat4_X_vec4(sunTranspose, vec3_to_vec4(world->sunVertices[i].position)));
        world->sunVertices[i].color= (vec4) {{0.0, 0.0, 0.0, 0.0}}; // Signal to the fragment shader to not do lighting for the sun
    }
    glBindVertexArray(world->sunVAO);
    glBindBuffer(GL_ARRAY_BUFFER, world->sunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(world->sunVertices), world->sunVertices, GL_STATIC_DRAW);
}

void world_render(World* world) {
    // Sun
    glBindVertexArray(world->sunVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->sunIBO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

    // Chunks
    for (int k= 0; k < WORLD_SIZE; k++) {
        for (int j= 0; j < WORLD_HEIGHT; j++) {
            for (int i= 0; i < WORLD_SIZE; i++) {
                chunkMesh_renderPrepare(&world->chunkArray[i][j][k]);
                chunkMesh_render(&world->chunkArray[i][j][k]);
            }
        }
    }
}

unsigned int world_getBlock(World* world, vec3 coordinate) {
    vec3 chunk= getChunkCoordFromWorldCoord(coordinate);
    vec3 withinChunk= vec3_floor(getPlayerPosWithinChunk(coordinate));

    if ((unsigned int) chunk.x >= WORLD_SIZE || (unsigned int) chunk.y >= WORLD_HEIGHT || (unsigned int) chunk.z >= WORLD_SIZE) return bt_Air;
    return world->chunkArray[(unsigned int) chunk.x][(unsigned int) chunk.y][(unsigned int) chunk.z].blockData[(unsigned int) CHUNK_DIM*CHUNK_DIM*(unsigned int)withinChunk.x + CHUNK_DIM*(unsigned int)withinChunk.y + (unsigned int)withinChunk.z];
}

// TODO: Complete
void world_blockBreak(World* world, vec3 playerPos, vec3 facingDirection, int radius) {
    return;
}

//ChunkMesh* world_getChunkMeshFromChunkCoords(World* world, vec3 chunkPos) {
//    return &world->chunkArray[(int) chunkPos.x][(int) chunkPos.y][(int) chunkPos.z];
//}

static vec3 getPlayerPosWithinChunk(vec3 position) {
    vec3 ans;
    ans.x= position.x >= 0 ? ((int) position.x % CHUNK_DIM) + (position.x - (int) position.x) : 16 + ((int) ((int) trunc(position.x) % CHUNK_DIM) + (position.x - trunc(position.x)));
    ans.y= position.y >= 0 ? ((int) position.y % CHUNK_DIM) + (position.y - (int) position.y) : 16 + ((int) ((int) trunc(position.y) % CHUNK_DIM) + (position.y - trunc(position.y)));
    ans.z= position.z >= 0 ? ((int) position.z % CHUNK_DIM) + (position.z - (int) position.z) : 16 + ((int) ((int) trunc(position.z) % CHUNK_DIM) + (position.z - trunc(position.z)));
    return (ans);
}

static vec3 getWorldCoordFromChunkCoord(vec3 chunkCoord, int x, int y, int z) {
    return (vec3) {{(chunkCoord.x)*CHUNK_DIM + x, (chunkCoord.y)*CHUNK_DIM + y, (chunkCoord.z)*CHUNK_DIM + z}};
}

static vec3 getChunkCoordFromWorldCoord(vec3 position) {
    return vec3_floor((vec3) {{(position.x)/CHUNK_DIM, (position.y)/CHUNK_DIM + 2, (position.z)/CHUNK_DIM}});
}
