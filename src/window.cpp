#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Window::Window(s32 w, s32 h) : m_width(w), m_height(h) {
  m_glWindow = nullptr;
  m_glWindow = glfwCreateWindow(m_width, m_height, "YeOldeEngine", nullptr, nullptr);
  if (!m_glWindow) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(m_glWindow);

  glViewport(0, 0, m_width, m_height);
}

Window::~Window(void) {
  glfwDestroyWindow(m_glWindow);
}
