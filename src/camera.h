#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
  glm::vec3 m_position;
  glm::vec3 m_target;
  glm::vec3 m_up;

  //  glm::vec2 m_rotation;
  glm::mat4 m_rotation;
  glm::mat4 m_matrix;
  glm::mat4 m_translation;

  bool m_updateMatrix;

public:
  Camera() :
      m_position{0.0f, 0.0f, 0.0f}, m_target{0.0f, 0.0f, -1.0f}, m_up{0.0f, 1.0f, 0.0f},
      m_matrix{1.0f}, m_rotation{1.0f}, m_translation{1.0f}, m_updateMatrix{true} {}
  Camera(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up) :
      m_position{position}, m_target{direction}, m_up{up}, m_matrix{1.0f}, m_rotation{1.0f},
      m_translation{1.0f}, m_updateMatrix{true} {}

  glm::mat4 CalculateMatrix();

  void Rotate(const f32 angle, const f32 horizontal, const f32 vertical);

  void Translate(const glm::vec3 &newPosition);
  void Translate(const glm::mat4 &translation);

  void Reset();
};

#endif // CAMERA_H
