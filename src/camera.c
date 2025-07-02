#include "camera.h"

static void doMovement_physics(Camera* cam, double deltaTime);
static void doMovement_noPhysics(Camera* cam, double deltaTime);

void scrollWheelCallback(GLFWwindow* window, double offsetX, double offsetY);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// To detect the rising edges of these keys
static bool mouse1Raised= false;
static bool lPressed= false;
static bool pPressed= false;
static bool spacePressed= false;
static bool shiftPressed= false;

// For framebuffer data
const float quadVertices[]= {
    -1.0, -1.0, 0.0, 0.0,
     1.0, -1.0, 1.0, 0.0,
    -1.0,  1.0, 0.0, 1.0,
     1.0,  1.0, 1.0, 1.0,
};
const unsigned int quadIndices[]= {
    0, 1, 3,
    0, 3, 2
};

static double scrollWheelPosition= 0;

Camera createCamera(Window* window, World* world) {
    Camera cam;
    cam.window= window;
    cam.world= world;
    cam.width= cam.window->width;
    cam.height= cam.window->height;
    cam.aspectRatio= cam.window->aspectRatio;

    cam.headBrightness= 0;
    cam.verticalVelocity= 0;
    cam.playerHeight= 1.9;
    cam.jumpStrength= 8;
    cam.reach= 5;

    glfwSetScrollCallback(window->windowHandle, scrollWheelCallback);
    glfwSetMouseButtonCallback(window->windowHandle, mouseButtonCallback);

    glGenFramebuffers(1, &cam.framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, cam.framebuffer);

    glActiveTexture(CAM_COLOR_BUFFER_TEXTURE);
    glGenTextures(1, &cam.textureColorBuffer);
    glBindTexture(GL_TEXTURE_2D, cam.textureColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cam.width, cam.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam.textureColorBuffer, 0);

    glActiveTexture(CAM_DEPTH_BUFFER_TEXTURE);
    glGenTextures(1, &cam.textureDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, cam.textureDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, cam.width, cam.height, 0,  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cam.textureDepthBuffer, 0);

    glGenVertexArrays(1, &cam.quadVAO);
    glBindVertexArray(cam.quadVAO);

    glGenBuffers(1, &cam.quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cam.quadVBO);

    glGenBuffers(1, &cam.quadIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cam.quadIBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*) (sizeof(float)*2));

    glBindBuffer(GL_ARRAY_BUFFER, cam.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), quadVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cam.quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), quadIndices, GL_STATIC_DRAW);

    cam.frameBufferShader= createGraphicsShaderProgram("shaders/framebuffer.vs", "shaders/framebuffer.fs");

    int UIx, UIy, UIch;
    unsigned char* UIdata= stbi_load("textures/UI.png", &UIx, &UIy, &UIch, 0);
    glActiveTexture(CAM_UI_TEXTURE);
    glGenTextures(1, &cam.UItexture);
    glBindTexture(GL_TEXTURE_2D, cam.UItexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, UIx, UIy, 0, GL_RGBA, GL_UNSIGNED_BYTE, UIdata);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(UIdata);
    return cam;
}

void camera_updateLens(Camera* cam, float fov, float nearZ, float farZ) {
    cam->fov= fov;
    cam->nearZ= nearZ;
    cam->farZ= farZ;

    cam->shaderProgram= createGraphicsShaderProgram("shaders/basic3D.vs", "shaders/basic3D.fs");
}

void camera_updatePlayerController(Camera* cam, float mouseSensitivity, float keyboardSensitivity, float horizontalAngle, float verticalAngle, vec3 startPos, bool doPhysics) {
    cam->mouseSensitivity= mouseSensitivity;
    cam->keyboardSensitivity= keyboardSensitivity;
    cam->horizontalAngle= horizontalAngle;
    cam->verticalAngle= verticalAngle;
    cam->position= startPos;
    cam->doPhysics= doPhysics;

    cam->focus= (vec3) {{cos(cam->verticalAngle)*sin(cam->horizontalAngle), sin(cam->verticalAngle), cos(cam->verticalAngle)*cos(cam->horizontalAngle)}},
    cam->right= vec3_normalize(vec3_cross(cam->focus, (vec3) {{0.0, 1.0, 0.0}}));
    cam->up= vec3_normalize(vec3_cross(cam->right, cam->focus));
}

bool camera_update(Camera* cam, double deltaTime) {
    // Capture mouse movements and state
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetInputMode(cam->window->windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    double mousePosX= cam->width/2.0, mousePosY= cam->height/2.0;
    if (glfwGetInputMode(cam->window->windowHandle, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
        glfwGetCursorPos(cam->window->windowHandle, &mousePosX, &mousePosY);
        glfwSetCursorPos(cam->window->windowHandle, cam->width/2.0, cam->height/2.0);
    }
    if (glfwGetMouseButton(cam->window->windowHandle, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        glfwSetInputMode(cam->window->windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(cam->window->windowHandle, cam->width/2.0, cam->height/2.0);
    }
    cam->horizontalAngle += cam->mouseSensitivity * deltaTime * (cam->width/2.0 - mousePosX);
    cam->verticalAngle += cam->mouseSensitivity * deltaTime * (cam->height/2.0 - mousePosY);
    // Basic view bobbing
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_W) == GLFW_PRESS
        || glfwGetKey(cam->window->windowHandle, GLFW_KEY_A) == GLFW_PRESS
        || glfwGetKey(cam->window->windowHandle, GLFW_KEY_S) == GLFW_PRESS
        || glfwGetKey(cam->window->windowHandle, GLFW_KEY_D) == GLFW_PRESS) {
        cam->verticalAngle *= 1 + 0.0025*sin(cam->world->timeElapsed*15);
    }
    // Limit horizonatalAngle -> [0, 2pi] and verticalAngle -> [-pi/2, pi/2]
    if (cam->horizontalAngle >= 2*PI) {
        cam->horizontalAngle -= 2*PI;
    } else if (cam->horizontalAngle <= 0) {
        cam->horizontalAngle += 2*PI;
    }
    if (cam->verticalAngle >= PI/2) {
        cam->verticalAngle= PI/2 - .0001;
    } else if (cam->verticalAngle <= -PI/2) {
        cam->verticalAngle= -PI/2 + .0001;
    }

    // Capture keyboard presses
    if (cam->doPhysics && cam->position.x-1 >= 0 && cam->position.z-1 >= 0 && cam->position.x+1 < CHUNK_DIM*WORLD_SIZE && cam->position.z+1 < CHUNK_DIM*WORLD_SIZE) {
        doMovement_physics(cam, deltaTime);
    } else {
        doMovement_noPhysics(cam, deltaTime);
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_P) == GLFW_PRESS && !pPressed) {
        pPressed= true;
        cam->doPhysics= !cam->doPhysics;
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_P) == GLFW_RELEASE) {
        pPressed= false;
    }
    world_getBlock(cam->world, cam->position);

    // Do mouse presses and block breaking
    // TODO: Switch to event system for mouse click
    if (mouse1Raised) {
        world_blockBreak(cam->world, cam->position, cam->focus, cam->reach);
        mouse1Raised= false;
    }

    // Capture mouse scroll as day length changes with a max dayLength of DEFAULT_DAYLIGHT_CYCLE...
    cam->world->dayLength= cam->world->dayLength*pow(1.5, -scrollWheelPosition) < DEFAULT_DAYLIGHT_CYCLE ? cam->world->dayLength*pow(1.5, -scrollWheelPosition) : cam->world->dayLength;
    if (cam->world->dayLength <= 0) {
        cam->world->dayLength= 1;
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_BACKSLASH) == GLFW_PRESS) {
        cam->world->dayLength= DEFAULT_DAYLIGHT_CYCLE;
    }
    scrollWheelPosition= 0;

    // Adjust head brightness
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_L) == GLFW_PRESS && !lPressed) {
        lPressed= true;
        if (cam->headBrightness == 0) {
            cam->headBrightness= 4;
        } else {
            cam->headBrightness= 0;
        }
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_L) == GLFW_RELEASE) {
        lPressed= false;
    }

    cam->focus= (vec3) {{cos(cam->verticalAngle)*sin(cam->horizontalAngle), sin(cam->verticalAngle), cos(cam->verticalAngle)*cos(cam->horizontalAngle)}},
    cam->right= vec3_normalize(vec3_cross(cam->focus, (vec3) {{0.0, 1.0, 0.0}}));
    cam->up= vec3_normalize(vec3_cross(cam->right, cam->focus));

    // Set the View and Projection matrix
    glUseProgram(cam->shaderProgram.shaderProgram);

    cam->viewMat= linAlg_View(cam->position, cam->focus, cam->up);
    unsigned int viewMatID= glGetUniformLocation(cam->shaderProgram.shaderProgram, "viewMat");
    glUniformMatrix4fv(viewMatID, 1, GL_TRUE, (float*) cam->viewMat.m);

    cam->projMat= linAlg_Perspective(cam->fov, cam->nearZ, cam->farZ, cam->window->aspectRatio);
    unsigned int projectionMatID= glGetUniformLocation(cam->shaderProgram.shaderProgram, "projectionMat");
    glUniformMatrix4fv(projectionMatID, 1, GL_TRUE, (float*) cam->projMat.m);

    unsigned int sunPosLoc= glGetUniformLocation(cam->shaderProgram.shaderProgram, "sunPos");
    glUniform3fv(sunPosLoc, 1, (float*) &cam->world->sunPosition);

    unsigned int playerPosLoc= glGetUniformLocation(cam->shaderProgram.shaderProgram, "playerPos");
    glUniform3fv(playerPosLoc, 1, (float*) &cam->position);

    unsigned int headBrightnessLoc= glGetUniformLocation(cam->shaderProgram.shaderProgram, "headBrightness");
    glUniform1f(headBrightnessLoc, cam->headBrightness);

    // Get ready to draw stuff
    if (cam->width != cam->window->width || cam->height != cam->window->height) { // If window has resized since last frame
        cam->width= cam->window->width;
        cam->height= cam->window->height;
        cam->aspectRatio= cam->window->aspectRatio;

        glDeleteFramebuffers(1, &cam->framebuffer);
        glDeleteTextures(1, &cam->textureColorBuffer);
        glDeleteTextures(1, &cam->textureDepthBuffer);

        glGenFramebuffers(1, &cam->framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);

        glActiveTexture(CAM_COLOR_BUFFER_TEXTURE);
        glGenTextures(1, &cam->textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, cam->textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cam->window->width, cam->window->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cam->textureColorBuffer, 0);

        glActiveTexture(CAM_DEPTH_BUFFER_TEXTURE);
        glGenTextures(1, &cam->textureDepthBuffer);
        glBindTexture(GL_TEXTURE_2D, cam->textureDepthBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, cam->window->width, cam->window->height, 0,  GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, cam->textureDepthBuffer, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, cam->framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    return glfwWindowShouldClose(cam->window->windowHandle);
}

bool camera_finish(Camera* cam) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(cam->frameBufferShader.shaderProgram);

    unsigned int starRotationID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "starRotation");
    mat4 starRotation= linAlg_Rotate(linAlg_Id(), (vec3) {{0.0, 1.0, -0.5}}, cam->world->dayNightCircle);
    glUniformMatrix4fv(starRotationID, 1, GL_TRUE, (float*) starRotation.m);
    unsigned int dayNightCircleLoc= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "dayNightCircle");
    glUniform1f(dayNightCircleLoc, cam->world->dayNightCircle);

    unsigned int sunPosID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "sunPos");
    unsigned int playerPosID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "playerPos");
    unsigned int viewDirID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "focus");
    unsigned int upDirID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "up");
    unsigned int rightDirID= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "right");
    glUniform3fv(sunPosID, 1, (float*) &cam->world->sunPosition);
    glUniform3fv(playerPosID, 1, (float*) &cam->position);
    glUniform3fv(viewDirID, 1, (float*) &cam->focus);
    glUniform3fv(upDirID, 1, (float*) &cam->up);
    glUniform3fv(rightDirID, 1, (float*) &cam->right);

    unsigned int widthLoc= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "width");
    unsigned int heightLoc= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "height");
    unsigned int fovLoc= glGetUniformLocation(cam->frameBufferShader.shaderProgram, "fov");
    glUniform1f(widthLoc, (float) cam->width);
    glUniform1f(heightLoc, (float) cam->height);
    glUniform1f(fovLoc, RAD(cam->fov));

    glBindVertexArray(cam->quadVAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cam->quadIBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

    glfwSwapBuffers(cam->window->windowHandle);
    return true;
}

// TODO: Fix this god awful garbage lol ;)
static void doMovement_physics(Camera* cam, double deltaTime) {
    // Do shifting
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        if (shiftPressed == false) { // Rising edge
            cam->position.y -= 0.4;
            cam->keyboardSensitivity /= 2;
        }
        cam->playerHeight= 1.5;
        shiftPressed= true;
    } else {
        if (shiftPressed == true) { // Falling edge
            cam->position.y += 0.4;
            cam->keyboardSensitivity *= 2;
        }
        cam->playerHeight= 1.9;
        shiftPressed= false;
    }
    vec3 feetPosition= (vec3) {{cam->position.x, cam->position.y - cam->playerHeight, cam->position.z}};
    unsigned int blockPosX= world_getBlock(cam->world, vec3_add(feetPosition, (vec3) {{ 0.5, 0.2, 0.0}}));
    unsigned int blockNegX= world_getBlock(cam->world, vec3_add(feetPosition, (vec3) {{-0.5, 0.2, 0.0}}));
    unsigned int blockPosZ= world_getBlock(cam->world, vec3_add(feetPosition, (vec3) {{ 0.0, 0.2, 0.5}}));
    unsigned int blockNegZ= world_getBlock(cam->world, vec3_add(feetPosition, (vec3) {{ 0.0, 0.2,-0.5}}));
    float movementX= cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
    float movementZ= cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_W) == GLFW_PRESS) {
        if ((blockNegX == 0 || movementX >= 0) && (blockPosX == 0 || movementX <= 0)) {
            cam->position.x += cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        }
        if ((blockPosZ == 0 || movementZ <= 0) && (blockNegZ == 0 || movementZ >= 0)) {
            cam->position.z += cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        }
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_S) == GLFW_PRESS) {
        if ((blockNegX == 0 || movementX <= 0) && (blockPosX == 0 || movementX >= 0)) {
            cam->position.x -= cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        }
        if ((blockPosZ == 0 || movementZ >= 0) && (blockNegZ == 0 || movementZ <= 0)) {
            cam->position.z -= cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        }
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_A) == GLFW_PRESS) {
        if ((blockNegX == 0 || movementZ >= 0) && (blockPosX == 0 || movementZ <= 0)) {
            cam->position.x += cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        }
        if ((blockPosZ == 0 || movementX >= 0) && (blockNegZ == 0 || movementX <= 0)) {
            cam->position.z -= cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        }
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_D) == GLFW_PRESS) {
        if ((blockNegX == 0 || movementZ <= 0) && (blockPosX == 0 || movementZ >= 0)) {
            cam->position.x -= cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        }
        if ((blockNegZ == 0 || movementX >= 0) && (blockPosZ == 0 || movementX <= 0)) {
            cam->position.z += cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        }
    }
    // Do falling
    cam->position.y += cam->verticalVelocity;
    if (world_getBlock(cam->world, feetPosition) == 0) {
        cam->verticalVelocity -= WORLD_GRAVITY * deltaTime;
    } else {
        cam->position.y -= cam->verticalVelocity;
        cam->verticalVelocity= 0;
    }
    // Do jumping
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS && world_getBlock(cam->world, feetPosition) != 0) {
        cam->position.y += cam->jumpStrength * deltaTime;
        cam->verticalVelocity= cam->jumpStrength * deltaTime;
    }
}

static void doMovement_noPhysics(Camera* cam, double deltaTime) {
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_W) == GLFW_PRESS) {
        cam->position.x += cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        cam->position.z += cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_S) == GLFW_PRESS) {
        cam->position.x -= cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
        cam->position.z -= cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_A) == GLFW_PRESS) {
        cam->position.x += cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        cam->position.z -= cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_D) == GLFW_PRESS) {
        cam->position.x -= cam->keyboardSensitivity * deltaTime * cos(cam->horizontalAngle);
        cam->position.z += cam->keyboardSensitivity * deltaTime * sin(cam->horizontalAngle);
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS) {
        cam->position.y += cam->keyboardSensitivity * deltaTime;
    }
    if (glfwGetKey(cam->window->windowHandle, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cam->position.y -= cam->keyboardSensitivity * deltaTime;
    }
}

void scrollWheelCallback(GLFWwindow* window, double offsetX, double offsetY) {
    scrollWheelPosition += offsetY;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        mouse1Raised= true;
    }
}
