#include "chunkMesh.h"

const unsigned int vertexStridePerFace= 4;
const unsigned int indexStridePerFace= 6;
const unsigned int numFaces= 6;
const vec3 cubeFaces[6]= {
    {{-1.0, 0.0, 0.0}},// -X
    {{ 0.0, 0.0, 1.0}},// +Z
    {{ 1.0, 0.0, 0.0}},// +X
    {{ 0.0, 0.0,-1.0}},// -Z
    {{ 0.0, 1.0, 0.0}},// +Y
    {{ 0.0,-1.0, 0.0}} // -Y
};
const Vertex cubeVertices[24]= {
    //-X-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 0.0, 0.0, 0.0}}, .normal = {{-1.0, 0.0, 0.0}}, .color = {{1.0, 0.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 0.0, 1.0}}, .normal = {{-1.0, 0.0, 0.0}}, .color = {{1.0, 0.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 1.0, 0.0}}, .normal = {{-1.0, 0.0, 0.0}}, .color = {{1.0, 0.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 1.0, 1.0}}, .normal = {{-1.0, 0.0, 0.0}}, .color = {{1.0, 0.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    //+Z-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 0.0, 0.0, 1.0}}, .normal = {{ 0.0, 0.0, 1.0}}, .color = {{1.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 0.0, 1.0}}, .normal = {{ 0.0, 0.0, 1.0}}, .color = {{1.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 1.0, 1.0}}, .normal = {{ 0.0, 0.0, 1.0}}, .color = {{1.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 1.0}}, .normal = {{ 0.0, 0.0, 1.0}}, .color = {{1.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    //+X-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 1.0, 0.0, 1.0}}, .normal = {{ 1.0, 0.0, 0.0}}, .color = {{0.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 0.0, 0.0}}, .normal = {{ 1.0, 0.0, 0.0}}, .color = {{0.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 1.0}}, .normal = {{ 1.0, 0.0, 0.0}}, .color = {{0.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 0.0}}, .normal = {{ 1.0, 0.0, 0.0}}, .color = {{0.0, 1.0, 0.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    //-Z-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 1.0, 0.0, 0.0}}, .normal = {{ 0.0, 0.0,-1.0}}, .color = {{0.0, 1.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 0.0, 0.0}}, .normal = {{ 0.0, 0.0,-1.0}}, .color = {{0.0, 1.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 0.0}}, .normal = {{ 0.0, 0.0,-1.0}}, .color = {{0.0, 1.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 1.0, 0.0}}, .normal = {{ 0.0, 0.0,-1.0}}, .color = {{0.0, 1.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    //+Y-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 0.0, 1.0, 0.0}}, .normal = {{ 0.0, 1.0, 0.0}}, .color = {{0.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 1.0, 1.0}}, .normal = {{ 0.0, 1.0, 0.0}}, .color = {{0.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 0.0}}, .normal = {{ 0.0, 1.0, 0.0}}, .color = {{0.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 1.0, 1.0}}, .normal = {{ 0.0, 1.0, 0.0}}, .color = {{0.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    //-Y-------------------------------------------------------------------------------------------------------------------------------
    {.position = {{ 0.0, 0.0, 0.0}}, .normal = {{ 0.0,-1.0, 0.0}}, .color = {{1.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 0.0, 0.0}}, .normal = {{ 0.0,-1.0, 0.0}}, .color = {{1.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 0.0, 0.0, 1.0}}, .normal = {{ 0.0,-1.0, 0.0}}, .color = {{1.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}},
    {.position = {{ 1.0, 0.0, 1.0}}, .normal = {{ 0.0,-1.0, 0.0}}, .color = {{1.0, 0.0, 1.0, 1.0}}, .texCoord = {{0.0, 0.0}}}
};
const unsigned int cubeIndices[36]= {
    //-X
    0, 1, 3,
    0, 3, 2,
    //+Z
    4, 5, 7,
    4, 7, 6,
    //+X
    8, 9, 11,
    8, 11, 10,
    //-Z
    12, 13, 15,
    12, 15, 14,
    //+Y
    16, 17, 19,
    16, 19, 18,
    //-Y
    20, 21, 23,
    20, 23, 22
};
const unsigned int wireFrameIndices[36]= {
    //-X
    0, 1,
    1, 3,
    3, 2,
    //+Z
    4, 5,
    5, 7,
    7, 6,
    //+X
    8, 9,
    9, 11,
    11, 10,
    //-Z
    12, 13,
    13, 15,
    15, 14,
    //+Y
    0, 0,
    0, 0,
    0, 0,
    //-Y
    0, 0,
    0, 0,
    0, 0
};

static void addFace(ChunkMesh* mesh, int x, int y, int z, int blockType, int face);

ChunkMesh createChunkMesh(vec3 chunkPos, int blockData[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM]) {
    ChunkMesh mesh;
    mesh.chunkPos= chunkPos;
    mesh.dirtyFlag= true;
    mesh.blockData= calloc(CHUNK_DIM*CHUNK_DIM*CHUNK_DIM, sizeof(int));
    if (mesh.blockData == NULL) {
        printf("Error allocating blockData array for chunk at %f, %f, %f.\n", chunkPos.x, chunkPos.y, chunkPos.z);
    } else {
        memcpy(mesh.blockData, blockData, CHUNK_DIM*CHUNK_DIM*CHUNK_DIM*sizeof(int));
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);

    glGenBuffers(1, &mesh.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 12*sizeof(float), (void*) (sizeof(float)*3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*) (sizeof(float)*6));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, 12*sizeof(float), (void*) (sizeof(float)*10));
    if (sizeof(Vertex) != 12*sizeof(float)) {
        printf("Vertex size changed! Update VAO!\n");
    }

    // Pre allocating vertices/indices space for an entire chunk
    // May need to remove "/2" but it should be fine like at least 98% of the time
    mesh.vertices= (Vertex*) calloc(CHUNK_DIM*CHUNK_DIM*CHUNK_DIM/2, sizeof(cubeVertices));
    mesh.indices= (unsigned int*) calloc(CHUNK_DIM*CHUNK_DIM*CHUNK_DIM/2, sizeof(cubeIndices));
    if (mesh.vertices == NULL) {
        printf("Error allocating vertices for chunk at %f, %f, %f.\n", chunkPos.x, chunkPos.y, chunkPos.z);
    }
    if (mesh.indices == NULL) {
        printf("Error allocating indices for chunk at %f, %f, %f.\n", chunkPos.x, chunkPos.y, chunkPos.z);
    }

    return mesh;
}

void chunkMesh_build(ChunkMesh* mesh) {
    mesh->numVertices= 0;
    mesh->numIndices= 0;

    for (int x= 0; x < CHUNK_DIM; x++) {
        for (int y= 0; y < CHUNK_DIM; y++) {
            for (int z= 0; z < CHUNK_DIM; z++) {
                // If block is transparent, skip
                if (mesh->blockData[CHUNK_DIM*CHUNK_DIM*x + CHUNK_DIM*y + z] == bt_Air) {
                    continue;
                }
                int blockId= mesh->blockData[CHUNK_DIM*CHUNK_DIM*x + CHUNK_DIM*y + z]; // Get block type
                for (int i= 0; i < (int) numFaces; i++) {
                    // Check if added face is on a border of the chunk
                    if (x + (int) cubeFaces[i].x < 0 || y + (int) cubeFaces[i].y < 0 || z + (int) cubeFaces[i].z < 0 || x + (int) cubeFaces[i].x == CHUNK_DIM || y + (int) cubeFaces[i].y == CHUNK_DIM || z + (int) cubeFaces[i].z == CHUNK_DIM) {
                        // TODO: Add check of adjacent chunks
                        addFace(mesh, x, y, z, blockId, i);
                    } else {
                        // Check if the block next to the added face is transparent
                        if (mesh->blockData[CHUNK_DIM*CHUNK_DIM*(x + (int) cubeFaces[i].x) + CHUNK_DIM*(y + (int) cubeFaces[i].y) + (z + (int) cubeFaces[i].z)] == bt_Air) {
                            addFace(mesh, x, y, z, blockId, i);
                        }
                    }
                }
            }
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->numVertices*sizeof(Vertex), mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->numIndices*sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);
}

void chunkMesh_renderPrepare(ChunkMesh* mesh) {
    if (mesh->dirtyFlag) {
        chunkMesh_build(mesh);
        mesh->dirtyFlag= false;
    }
}

void chunkMesh_render(ChunkMesh* mesh) {
    glBindVertexArray(mesh->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);
}

static void addFace(ChunkMesh* mesh, int x, int y, int z, int blockType, int face) {
    // Copy vertices for the added face
    memcpy(&mesh->vertices[mesh->numVertices], &cubeVertices[vertexStridePerFace*face], sizeof(Vertex)*vertexStridePerFace);
    memcpy(&mesh->indices[mesh->numIndices], cubeIndices, sizeof(unsigned int)*indexStridePerFace);
    // Translate and modify texture coordinates for each vertex in the added face
    for (int i= 0; i < (int) vertexStridePerFace; i++) {
        mesh->vertices[mesh->numVertices+i].position= vec4_to_vec3(mat4_X_vec4(linAlg_Translate(linAlg_Id(), x + 16*mesh->chunkPos.x, y + 16*mesh->chunkPos.y, z + 16*mesh->chunkPos.z), vec3_to_vec4(mesh->vertices[mesh->numVertices+i].position)));
        if (face == 4) { // Top texture (0)
            mesh->vertices[mesh->numVertices+i].texCoord.s= 0 + 0.03125 * (i%2) + /*Block Offset*/ 3*0.03125*(blockType-1);
            mesh->vertices[mesh->numVertices+i].texCoord.t= 0 + 0.03125 * (i/2);
        } else if (face == 5) { // Bottom texture (2)
            mesh->vertices[mesh->numVertices+i].texCoord.s= 2*0.03125 + 0.03125 * (i%2) + /*Block Offset*/ 3*0.03125*(blockType-1);
            mesh->vertices[mesh->numVertices+i].texCoord.t= 0 + 0.03125 * (i/2);
        } else { // Side texture (1)
            mesh->vertices[mesh->numVertices+i].texCoord.s= 1*0.03125 + 0.03125 * (i%2) + /*Block Offset*/ 3*0.03125*(blockType-1);
            mesh->vertices[mesh->numVertices+i].texCoord.t= 0 + 0.03125 * (i/2);
        }
    }
    // Translate indices for the added face
    for (int i= 0; i < (int) indexStridePerFace; i += 1) {
        mesh->indices[mesh->numIndices+i] += mesh->numVertices;
    }
    mesh->numVertices += vertexStridePerFace;
    mesh->numIndices += indexStridePerFace;
}
