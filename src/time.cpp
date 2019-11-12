#include "time.h"

#include "GLFW/glfw3.h"

Time::Time() : m_previousTime{(f32)glfwGetTime()}, m_currentTime{m_previousTime}, m_delta{0} {
}

void Time::Step() {
  m_previousTime = m_currentTime;
  m_currentTime = (f32)glfwGetTime();
  m_delta = m_currentTime - m_previousTime;
}

void Time::SetTime(f32 t) {
  glfwSetTime((f64)t);
  m_currentTime = t;
  Step();
}