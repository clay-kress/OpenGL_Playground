#ifndef SHADER_H
#define SHADER_H

#ifdef _WIN32
#include "lib/glew32.h"
#else
#include <GL/gl.h>
#endif
#include "lib/glfw3.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned int shaderProgram;
} GraphicsShader;

GraphicsShader createGraphicsShaderProgram(const char* vShader, const char* fShader);

#endif
