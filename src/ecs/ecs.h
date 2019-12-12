#ifndef ECS_H
#define ECS_H

#include "../common.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

class Shader;
struct BoundingBox;
namespace Renderer {
enum class Command;
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
  //  std::vector<ShaderData> shaderData = {};
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
  //  BoundingBox boundingBox = {};
};

struct NodeMotion {
  glm::vec3 translations = {};
  glm::vec3 rotations = {};
};

constexpr u64 COUNT_MASK = 0x0000000000ff0000;
constexpr u64 INDEX_MASK = 0x000000000000ffff;
constexpr u16 ID_MAX = 0xffff;

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
  using CompTuple = std::tuple<std::array<Physics, ID_MAX>, std::array<Renderable, ID_MAX>,
      std::array<Transform, ID_MAX>, std::array<Shaterable, ID_MAX>, std::array<Mesh, ID_MAX>,
      std::array<Collidable, ID_MAX>>;
  CompTuple m_components;

public:
  ComponentManager() = default;
  ~ComponentManager();

  EntityID CreateEntity(EntityID type);

  void DestroyEntity(EntityID id);

  template<typename T>
  T *GetComponent(EntityID id) {
    auto component = std::get<std::vector<T *>>(m_components);
    u32 index = id & INDEX_MASK;
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
