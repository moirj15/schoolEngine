#pragma once

#include "../common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <string>
#include <vector>
#include <unordered_set>
#include "../TriangleNeighbor.h"
#include "../aabb.h"

namespace renderer {
enum class Command;
}
namespace ecs {
struct Component {
  virtual ~Component() = default;
};

struct PhysicsComponent : public Component {
  glm::vec3 m_velocity = {};
  glm::vec3 m_momentum = {};
  glm::quat m_angularVelocity = {};
  glm::quat m_angularMomentum = {};
  f32 m_mass = 0.0f;
  f32 m_frictionCoef = 0.0f;
};

struct Edge {
  u32 start;
  u32 end;

  bool operator==(const Edge &o) const { return start == o.start && end == o.end; }
  Edge() = default;
};


struct DECLComponent : public Component {
  std::vector<Triangle *> m_triangles;
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
  u64 m_id = 0;
  bool m_hasCollided = false;
  f32 m_collisionTime = 0.0f;
//  AABB m_boundingBox = {};
  f32 m_boundingSphereRadius = 0.0f;
  std::unordered_set<u32> m_collidedTriangles;
};

struct NodeMotionComponent : public Component {
  glm::vec3 m_translations = {};
  glm::vec3 m_rotations = {};
};

} // namespace ecs
