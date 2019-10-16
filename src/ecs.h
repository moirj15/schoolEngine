#ifndef ECS_H
#define ECS_H

#include "boundingbox.h"
#include "common.h"
#include "renderer.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>
#include <tuple>
#include <vector>

namespace Renderer {
enum Command;
}

struct ShaderData;
class VertexArray;

namespace ECS {

using EntityID = u32;

struct Entity {
  EntityID id;

  Entity(EntityID eid) : id(eid) {}
  virtual ~Entity() = default;
};

struct Physics {
  glm::vec3 velocity = {};
  glm::quat angularVelocity = {};
  glm::quat angularMomentum = {};
  glm::vec3 momentum = {};
  f32 mass = 0.0f;
  f32 frictionCoef = 0.0f;
};

struct Renderable {
  std::vector<Renderer::Command> commands = {};
  std::vector<ShaderData> shaderData = {};
  VertexArray *vertexArray = nullptr;
  Shader *shader = nullptr;
};

struct Transform {
  glm::vec3 position = {};
  glm::quat rotation = {};
};

struct Shaterable {
  bool hasShatered;
};

struct Mesh {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  u32 vertexSize = 3;
};

struct Collidable {
  bool hasCollided;
  EntityID collidedEntity;
  f32 collisionTime;
  BoundingBox boundingBox;
};

enum class Type : u32 {
  Renderable = 1 << 24,
  Physics = 1 << 25,
  Transform = 1 << 26,
  Shaterable = 1 << 27,
  Mesh = 1 << 28,
  Collidable = 1 << 29
};

class ComponentManager {
  using CompTuple =
      std::tuple<std::vector<Physics *>, std::vector<Renderable *>,
          std::vector<Transform *>, std::vector<Shaterable *>,
          std::vector<Mesh *>, std::vector<Collidable *>>;
  CompTuple m_components;

public:
  ComponentManager() :
      m_components{{1024, nullptr}, {1024, nullptr}, {1024, nullptr},
          {1024, nullptr}, {1024, nullptr}, {1024, nullptr}} {}
  ~ComponentManager();

  EntityID CreateEntity(u32 type);

  void DestroyEntity(EntityID id);

  template<typename T>
  T *GetComponent(EntityID id) {
    auto component = std::get<std::vector<T *>>(m_components);
    if (id < component.size()) { return component[id]; }
    return nullptr;
  }

  template<typename T>
  std::vector<T *> GetComponents() {
    return std::get<std::vector<T *>>(m_components);
  }

private:
  EntityID NextID();
};

} // namespace ECS

#endif
