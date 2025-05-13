#include "window.h"

static bool glfwInititalized= false;

static Window* activeWindow;

static void windowResizeCallback(GLFWwindow* window, int width, int height) {
    if (window != activeWindow->windowHandle) {
        printf("not able to resize inactive window!\n");
        return;
    }
    activeWindow->width= width;
    activeWindow->height= height;
    activeWindow->aspectRatio= (float) activeWindow->width / (float) activeWindow->height;
    glViewport(0, 0, activeWindow->width, activeWindow->height);
}

Window createWindow(unsigned int width, unsigned int height, const char* name, bool vsync, bool fullScreen) {
    Window win;
    win.width= width;
    win.height= height;
    win.aspectRatio= (float) win.width / (float) win.height;
    win.name= name;

    // Create the window
    if (!glfwInititalized) {
        if (glfwInit() == GLFW_FALSE) {
            fprintf(stderr, "glfwInit failed. exiting\n");
            exit(1);
        }
        glfwInititalized= true;
    }
    win.monitor= fullScreen ? glfwGetPrimaryMonitor() : NULL;
    if (win.monitor != NULL) {
        int x, y;
        glfwGetMonitorWorkarea(win.monitor, &x, &y, (int*) &win.width, (int*) &win.height);
        win.aspectRatio= (float) win.width / (float) win.height;
    }
    if ((win.windowHandle= glfwCreateWindow(win.width, win.height, win.name, win.monitor, NULL)) == NULL) {
        fprintf(stderr, "glfwCreateWindow failed. exiting\n");
        exit(1);
    }
    glfwSetInputMode(win.windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowSizeCallback(win.windowHandle, windowResizeCallback);
    glfwMakeContextCurrent(win.windowHandle);
    glfwSwapInterval(vsync);

    // Initialize OpenGL
#ifdef USEGLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "glewInit failed\n");
        exit(1);
    }
#endif
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Get OpenGL and Graphics version stats
    const unsigned char* vendor= glGetString(GL_VENDOR);
    const unsigned char* version= glGetString(GL_VERSION);
    const unsigned char* renderer= glGetString(GL_RENDERER);
    int majorV; glGetIntegerv(GL_MAJOR_VERSION, &majorV);
    int minorV; glGetIntegerv(GL_MINOR_VERSION, &minorV);
    printf("Graphics Card Info: %s, %s, %s\nOpenGL version: %d.%d\n", renderer, version, vendor, majorV, minorV);

    return win;
}

void window_makeCurrentContext(Window* win) {
    activeWindow= win;
    glfwMakeContextCurrent(win->windowHandle);
}
