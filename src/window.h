#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

struct GLFWwindow;

struct Window {
  s32 m_width;
  s32 m_height;
  GLFWwindow *m_glWindow;

  Window(s32 w, s32 h);
  ~Window(void);
};

#endif
