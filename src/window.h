#ifndef WINDOW_H
#define WINDOW_H

#ifndef __APPLE__
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include "common.h"


struct Window {
    s32 m_width;
    s32 m_height;
    GLFWwindow *m_glWindow;

    Window(s32 w, s32 h);
    ~Window(void);
};

#endif
