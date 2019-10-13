#ifndef ECS_H
#define ECS_H

#include "common.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <optional>
#include <vector>

namespace Renderer {
enum Command;
}

struct ShaderData;
class VertexArray;
class Shader;

namespace ECS {

using EntityID = u32;

struct Entity {
  EntityID id;

  Entity(EntityID eid) : id(eid) {}
  virtual ~Entity() = default;
};

struct Physics : public Entity {
  glm::vec3 velocity = {};
  glm::quat angularVelocity = {};
  glm::quat angularMomentum = {};
  glm::vec3 momentum = {};
  f32 mass = 0.0f;

  Physics(EntityID eid) : Entity{eid} {}
};

struct Renderable : public Entity {
  std::vector<Renderer::Command> commands;
  std::vector<ShaderData> shaderData;
  VertexArray *vertexArray;
  Shader *shader;
  Renderable(EntityID eid) : Entity{eid} {}
};

struct Transform : public Entity {
  glm::vec3 position = {};
  glm::quat rotation = {};
  Transform(EntityID eid) : Entity{eid} {}
};

enum class Type : u32 {
  Renderable = 1 << 24,
  Physics = 1 << 25,
  Transform = 1 << 26,
};

void Init();

void DeInit();

EntityID CreateEntity(u32 type);

std::optional<Physics *> GetPhysicsComponent(EntityID id);

std::vector<Physics *> GetPhysics();

#include <tuple>
class ComponentManager {
  using CompTuple = std::tuple<std::vector<Physics *>,
      std::vector<Renderable *>, std::vector<Transform *>>;
  CompTuple m_components;

public:
  ComponentManager() = default;
  ~ComponentManager();

  EntityID CreateEntity(u32 type);

  template<typename T>
  T *GetComponent(EntityID id) {
    auto component = std::get<T>(m_components);
    if (id < component.size()) { return component[id]; }
    return nullptr;
  }

  template<typename T>
  std::vector<T *> GetComponents() {
    return std::get<T>(m_components);
  }

private:
  EntityID NextID();
};

} // namespace ECS

#endif
