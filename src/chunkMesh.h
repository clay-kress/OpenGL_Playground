#ifndef MESH_H
#define MESH_H

#include "data/linAlg.h"

#ifdef _WIN32
#include "lib/glew32.h"
#else
#include <GL/gl.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define CHUNK_DIM 16
//#define WIRE_FRAME

typedef enum {
    bt_Air= 0, bt_Sun, bt_Grass, bt_Dirt, bt_Stone, bt_Water
} BlockType;

typedef struct {
    vec3 position;
    vec3 normal;
    vec4 color;
    vec2 texCoord;
} Vertex;

extern const vec3 cubeFaces[6];
extern const Vertex cubeVertices[24];
extern const unsigned int cubeIndices[36];
extern const unsigned int wireFrameIndices[36];

typedef struct {
    vec3 chunkPos; // position on the chunk grid (world coordinates = 16*worldPos)
    bool dirtyFlag;
    int* blockData; // heap memory for the block type data. will be of size CHUNK_DIM * CHUNK_DIM * CHUNK_DIM
    Vertex* vertices;
    unsigned int* indices;
    unsigned int numVertices, numIndices;
    unsigned int vbo, ibo, vao;
} ChunkMesh;

ChunkMesh createChunkMesh(vec3 chunkPos, int blockData[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM]);
void chunkMesh_build(ChunkMesh* mesh);
void chunkMesh_renderPrepare(ChunkMesh* mesh);
void chunkMesh_render(ChunkMesh* mesh);

#endif
