#ifndef WINDOW_H
#define WINDOW_H

#ifdef _WIN32
#include "lib/glew32.h"
#else
#include <GL/gl.h>
#endif
#include "lib/glfw3.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    GLFWwindow* windowHandle;
    GLFWmonitor* monitor;
    unsigned int width, height;
    float aspectRatio;
    const char* name;
} Window;

Window createWindow(unsigned int width, unsigned int height, const char* name, bool vsync, bool fullScreen);
void window_makeCurrentContext(Window* win);

#endif
