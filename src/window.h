#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

struct GLFWwindow;

struct Window {
  s32 m_width;
  s32 m_height;
  GLFWwindow *m_glWindow;

  /**
   * Constructor.
   * @param w: The width of the window.
   * @param h: The height of the window.
   */
  Window(s32 w, s32 h);

  /// Destructor
  ~Window(void);
};

#endif
