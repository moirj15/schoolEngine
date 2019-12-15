#pragma once

#include "../common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <vector>

namespace renderer {
enum class Command;
}
namespace ecs {
struct Component {
  virtual ~Component() = default;
};

struct PhysicsComponent : public Component {
  glm::vec3 m_velocity = {};
  glm::quat m_angularVelocity = {};
  glm::quat m_angularMomentum = {};
  glm::vec3 m_momentum = {};
  f32 m_mass = 0.0f;
  f32 m_frictionCoef = 0.0f;
};


struct RenderableComponent : public Component {
  std::string m_texture;
  std::string m_shader;
  std::vector<renderer::Command> m_commands;
};

struct TransformComponent : public Component {
  glm::vec3 m_position = {};
  glm::quat m_rotation = {};
};

struct ShaterableComponent : public Component {
  bool m_hasShatered = false;
};

struct MeshComponent : public Component {
  std::vector<f32> m_vertecies = {};
  std::vector<u32> m_connections = {};
  std::vector<f32> m_normals = {};
  u32 m_vertexSize = 3;
};

struct CollidableComponent : public Component {
  bool m_hasCollided = false;
  // EntityID collidedEntity = 0;
  f32 m_collisionTime = 0.0f;
  //  AABB boundingBox = {};
};

struct NodeMotionComponent : public Component {
  glm::vec3 m_translations = {};
  glm::vec3 m_rotations = {};
};

} // namespace ecs
