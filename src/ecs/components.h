#pragma once

#include "../common.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace ecs {
struct Component {
  virtual ~Component() = default;
};

struct PhysicsComponent : public Component {
  glm::vec3 velocity = {};
  glm::quat angularVelocity = {};
  glm::quat angularMomentum = {};
  glm::vec3 momentum = {};
  f32 mass = 0.0f;
  f32 frictionCoef = 0.0f;
};

struct RenderableComponent : public Component {
  ////std::vector<Renderer::Command> commands = {};
  ////  std::vector<ShaderData> shaderData = {};
  // VertexArray *vertexArray = nullptr;
  // Shader *shader = nullptr;
};

struct TransformComponent : public Component {
  glm::vec3 position = {};
  glm::quat rotation = {};
};

struct ShaterableComponent : public Component {
  bool hasShatered = false;
};

struct MeshComponent : public Component {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  u32 vertexSize = 3;
};

struct CollidableComponent : public Component {
  bool hasCollided = false;
  // EntityID collidedEntity = 0;
  f32 collisionTime = 0.0f;
  //  BoundingBox boundingBox = {};
};

struct NodeMotionComponent : public Component {
  glm::vec3 translations = {};
  glm::vec3 rotations = {};
};

} // namespace ecs
