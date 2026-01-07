/* Sandbox (Cubed) by Clay Kress
 * Helpful Sources:
 *     OpenGL (Framebuffers, Cubemaps): https://learnopengl.com/
 *     Perlin noise for clouds: https://rtouti.github.io/graphics/perlin-noise-algorithm
 *
 * Inspired by (but not copied from): https://github.com/jdah/minecraft-weekend/tree/master
 *
 */

#ifdef USEGLEW
#include "src/lib/glew32.h"
#else
#include <GL/gl.h>
#endif
#include "src/lib/glfw3.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <stdlib.h>
#include <stdio.h>

#include "src/data/linAlg.h"
#include "src/world.h"
#include "src/camera.h"
#include "src/window.h"
#include "src/shader.h"

#define DEFAULT_WIDTH 960
#define DEFAULT_HEIGHT 540
#define DEFAULT_NAME "Application"

int main(int argc, char* argv[]) {
    printf("%s\n", "Hello, World!");

    bool vsync= true, fullscreen= false;

    float mouseSensitivity= 0.3;
    float keyboardSensitivity= 5;

    float horizontalAngle= -PI/2.0, verticalAngle= PI/8.0;
    vec3 startPos= {{36.0, 15.0, 32.0}};

    Window window= createWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_NAME, vsync, fullscreen);
    window_makeCurrentContext(&window);

    World world= createWorld();

    Camera player= createCamera(&window, &world);
    camera_updateLens(&player, 75, 0.01, 1000);
    camera_updatePlayerController(&player, mouseSensitivity, keyboardSensitivity, horizontalAngle, verticalAngle, startPos, true);

    double startTime= glfwGetTime(), lastTime= glfwGetTime();
    double endTime= startTime;
    int frameCount= 0;

    double deltaTime= 1, timeElapsed= 0;
    while (!camera_update(&player, deltaTime)) {
        deltaTime= endTime-startTime;
        timeElapsed += deltaTime;
        if (frameCount == 120) {
            double currentTime= glfwGetTime();
            double timeSinceLastPrint= currentTime-lastTime;
            lastTime= currentTime;
            //printf("Time Elapsed= %f\n    SPF= %f, FPS= %f\n", timeElapsed, timeSinceLastPrint/frameCount, 1.0/(timeSinceLastPrint/frameCount));
            frameCount= 0;
        }
        frameCount++;

        world_renderPrepare(&world, deltaTime, player.position);
        world_render(&world);
        camera_finish(&player);

        startTime= endTime; // Previous endTime.
        endTime= glfwGetTime();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}

// TODO: Fix player raycasting for block breaking/placing
//        if (mouse1Pressed && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
//            mouse1Pressed= false;
//        }
//        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !mouse1Pressed) {
//            mouse1Pressed= true;
//            vec3 blockLocation= {0};
//            vec3 rayPos= pos;
//            for (int i= 0; i < 6; i++) {
//                if (round(rayPos.x) >= 0 && round(rayPos.x) < 16 && round(rayPos.y) >= 0 && round(rayPos.y) < 16 && round(rayPos.z) >= 0 && round(rayPos.z) < 16) {
//                    if (blocks[(unsigned int) round(rayPos.x)][(unsigned int) round(rayPos.y)][(unsigned int) round(rayPos.z)] != 0) {
//                        blockLocation= vec3_round(rayPos);
//                        blocks[(unsigned int) round(rayPos.x)][(unsigned int) round(rayPos.y)][(unsigned int) round(rayPos.z)]= 0;
//                        break;
//                    }
//                }
//                float nearestPlaneAngle= fmax(
//                    fmax(fmax(vec3_dot(focus, (vec3) { 1.0, 0.0, 0.0}), vec3_dot(focus, (vec3) {0.0, 1.0, 0.0})), vec3_dot(focus, (vec3) {0.0, 0.0, 1.0})),
//                    fmax(fmax(vec3_dot(focus, (vec3) {-1.0, 0.0, 0.0}), vec3_dot(focus, (vec3) {0.0,-1.0, 0.0})), vec3_dot(focus, (vec3) {0.0, 0.0,-1.0})));
//                rayPos= vec3_add(rayPos, vec3_multiply(focus, 0.5/nearestPlaneAngle));
//            }
//            Vertex* cubeV= blockVertexPtr[(int) blockLocation.x][(int) blockLocation.y][(int) blockLocation.z];
//            unsigned int* cubeI= blockIndexPtr[(int) blockLocation.x][(int) blockLocation.y][(int) blockLocation.z];
//            for (int i= 0; i < sizeof(cubeVertices)/sizeof(Vertex); i++) {
//                cubeV[i]= (Vertex) {0};
//            }
//#ifdef WIRE_FRAME
//            for (int i= 0; i < sizeof(wireFrameIndices)/sizeof(unsigned int); i++) {
//                cubeI[i]= (unsigned int) {0};
//            }
//#else
//            for (int i= 0; i < (sizeof(cubeIndices)/sizeof(unsigned int)); i++) {
//                cubeI[i]= (unsigned int) {0};
//            }
//#endif
//        }
