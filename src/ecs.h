#ifndef ECS_H
#define ECS_H

#include "boundingbox.h"
#include "common.h"
#include "renderer.h"
#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

namespace Renderer {
enum Command;
}

struct ShaderData;
class VertexArray;

namespace ECS {

using EntityID = u64;

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
  bool hasShatered = false;
};

struct Mesh {
  std::vector<f32> vertecies = {};
  std::vector<u32> connections = {};
  std::vector<f32> normals = {};
  u32 vertexSize = 3;
};

struct Collidable {
  bool hasCollided = false;
  EntityID collidedEntity = 0;
  f32 collisionTime = 0.0f;
  BoundingBox boundingBox = {};
};

struct NodeMotion {
  glm::vec3 translations = {};
  glm::vec3 rotations = {};
};

struct Skeleton {
  u32 allowedMotions = 0;
  glm::vec3 offset = {};
  std::string name = {};
  std::vector<NodeMotion> motions;
  std::vector<std::unique_ptr<Skeleton>> children = {};
  Skeleton() = default;
  Skeleton(const std::string &n) : name{n} {}

  std::vector<Skeleton *> ToList();
};

enum class Type : u64 {
  Renderable = 1 << 24,
  Physics = 1 << 25,
  Transform = 1 << 26,
  Shaterable = 1 << 27,
  Mesh = 1 << 28,
  Collidable = 1 << 29,
  Skeleton = 1 << 30,
};

class ComponentManager {
  using CompTuple = std::tuple<std::vector<Physics *>,
      std::vector<Renderable *>, std::vector<Transform *>,
      std::vector<Shaterable *>, std::vector<Mesh *>, std::vector<Collidable *>,
      std::vector<Skeleton *>>;
  CompTuple m_components;

public:
  ComponentManager() :
      m_components{{1024 * 10, nullptr}, {1024 * 10, nullptr},
          {1024 * 10, nullptr}, {1024 * 10, nullptr}, {1024 * 10, nullptr},
          {1024 * 10, nullptr}, {1024 * 10, nullptr}} {}
  ~ComponentManager();

  EntityID CreateEntity(EntityID type);

  void DestroyEntity(EntityID id);

  template<typename T>
  T *GetComponent(EntityID id) {
    auto component = std::get<std::vector<T *>>(m_components);
    u32 index = id & 0x0000ffff;
    if (index < component.size()) {
      return component[index];
    }
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
