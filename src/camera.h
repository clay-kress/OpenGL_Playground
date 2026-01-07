#ifndef CAMERA_H
#define CAMERA_H

#include "data/linAlg.h"

#include "window.h"
#include "shader.h"
#include "world.h"

#ifdef _WIN32
#include "lib/glew32.h"
#else
#include <GL/gl.h>
#endif
#include "lib/glfw3.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef struct {
    Window* window;
    World* world;
    // Rendering data
    mat4 viewMat, projMat;
    GraphicsShader shaderProgram, frameBufferShader;
    unsigned int framebuffer, textureColorBuffer, textureDepthBuffer;
    unsigned int quadVAO, quadVBO, quadIBO;
    unsigned int UItexture;
    // Camera Control
    unsigned int width, height; // Duplicates from "window.h" to keep track of window resizes. Fix using window resize event
    float aspectRatio; // Duplicate from "window.h" to keep track of window resizes. Fix using window resize event
    float fov, nearZ, farZ;
    // Player Control
    float mouseSensitivity, keyboardSensitivity;
    float horizontalAngle, verticalAngle;
    vec3 position, focus, right, up;
    // Messing around
    float headBrightness;
    float verticalVelocity;
    float playerHeight;
    float jumpStrength;
    int reach;
    bool doPhysics;
} Camera;

Camera createCamera(Window* window, World* world);
void camera_updateLens(Camera* cam, float fov, float nearZ, float farZ);
void camera_updatePlayerController(Camera* cam, float mouseSensitivity, float keyboardSensitivity, float horizontalAngle, float verticalAngle, vec3 startPos, bool doPhysics);
bool camera_update(Camera* cam, double deltaTime);
bool camera_finish(Camera* cam);

#endif
