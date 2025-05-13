#ifndef WORLD_H
#define WORLD_H

#include "chunkMesh.h"
#include "shader.h"

#include "stb_image.h"

#define WORLD_SIZE 10
#define WORLD_HEIGHT 10
#define WORLD_GRAVITY 0.49
#define WORLD_START_TIME 0 // [0-1]
#define DEFAULT_DAYLIGHT_CYCLE (60*10) // seconds= 60*minutes

#define WORLD_TEXTURE GL_TEXTURE0
#define CAM_UI_TEXTURE GL_TEXTURE1
#define CAM_COLOR_BUFFER_TEXTURE GL_TEXTURE2
#define CAM_DEPTH_BUFFER_TEXTURE GL_TEXTURE3

typedef struct {
    // Terrain Data
    ChunkMesh chunkArray[WORLD_SIZE][WORLD_HEIGHT][WORLD_SIZE]; // May need to replace with heap allocated memory if world gets too big
    // Sun data
    Vertex sunVertices[24];
    unsigned int sunVAO, sunVBO, sunIBO;
    vec3 sunPosition;
    float dayNightCircle; // Midday is 0, 2pi, etc.
    float dayLength; // Length of (full) day in seconds
    float timeElapsed;
    // Texture data (blockAtlas & starMap)
    int blockAtlasX, blockAtlasY, blockAtlasChannels;
    unsigned char* blockAtlas;
    unsigned int glBlockAtlas;
    int starMapX, starMapY, starMapChannels;
    unsigned char* starMap;
    unsigned char* northStarMap;
    unsigned int glStarMap; // Cubemap
} World;

World createWorld(void);
void world_renderPrepare(World* world, double deltaTime, vec3 playerPos);
void world_render(World* world);
void world_blockBreak(World* world, vec3 playerPos, vec3 facingDirection);
unsigned int world_getBlock(World* world, vec3 coordinate);
ChunkMesh* world_getChunkMeshFromChunkCoords(World* world, vec3 chunkPos);

#endif
