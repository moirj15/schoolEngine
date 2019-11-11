#include "camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
glm::mat4 Camera::CalculateMatrix() {
  if (m_updateMatrix) {
    m_updateMatrix = false;
    m_position = m_translation * glm::vec4{m_position, 1.0f};
    m_direction = m_rotation * glm::vec4{m_direction, 1.0f};
    m_matrix = glm::lookAt(m_position, m_direction, m_up);
    return m_matrix;
  } else {
    return m_matrix;
  }
}

void Camera::Rotate(const f32 angle, const glm::vec3 axis) {
  m_updateMatrix = true;
  m_rotation = glm::rotate(glm::radians(angle), axis);
}
void Camera::Rotate(const glm::quat &rotation) {
  m_updateMatrix = true;
  m_rotation = glm::toMat4(rotation);
}
void Camera::Rotate(const glm::mat4 &rotation) {
  m_updateMatrix = true;
  m_rotation = rotation;
}

void Camera::Translate(const glm::vec3 &newPosition) {
  m_updateMatrix = true;
  m_translation = glm::translate(newPosition);
}
void Camera::Translate(const glm::mat4 &translation) {
  m_updateMatrix = true;
  m_translation = translation;
}

void Camera::Reset() {
  m_position = {0.0f, 0.0f, 0.0f};
  m_direction = {0.0f, 0.0f, -1.0f};
  m_up = {0.0f, 1.0f, 0.0f};
  m_matrix = glm::mat4{1.0f};
  m_rotation = glm::mat4{1.0f};
  m_updateMatrix = true;
}