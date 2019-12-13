#ifndef ECS_H
#define ECS_H

#include "../common.h"
#include "system.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class Shader;
struct BoundingBox;
namespace Renderer {
enum class Command;
}

struct ShaderData;
class VertexArray;

namespace ecs {

struct Component;
struct PhysicsComponent;
struct RenderableComponent;
struct TransformComponent;
struct ShaterableComponent;
struct MeshComponent;
struct CollidableComponent;

using EntityID = u64;

constexpr u64 COUNT_MASK = 0x0000000000ff0000;
constexpr u64 INDEX_MASK = 0x000000000000ffff;
constexpr u16 ID_MAX = 0xffff;
constexpr u64 FREE_ID = 0x8000000000000000;

enum class Type : u64 {
  Renderable = 1 << 24,
  Physics = 1 << 25,
  Transform = 1 << 26,
  Shaterable = 1 << 27,
  Mesh = 1 << 28,
  Collidable = 1 << 29,
  Skeleton = 1 << 30,
};

enum class TupleType : u64 {
  Destructable = (u64)Type::Renderable | (u64)Type::Physics | (u64)Type::Transform
                 | (u64)Type::Shaterable | (u64)Type::Mesh | (u64)Type::Collidable,
  RenderableTuple = (u64)Type::Renderable | (u64)Type::Mesh | (u64)Type::Transform,
};

struct Entity {
  EntityID m_id;
  std::vector<Component *> m_components;
};

class WorldSystem : public System {
  std::unordered_map<EntityID, Entity *> m_entities;
  std::vector<System *> m_systems;
  std::array<EntityID, ID_MAX> m_entityIDs;

  using CompTuple =
      std::tuple<std::array<PhysicsComponent *, ID_MAX>, std::array<RenderableComponent *, ID_MAX>,
          std::array<TransformComponent *, ID_MAX>, std::array<ShaterableComponent *, ID_MAX>,
          std::array<MeshComponent *, ID_MAX>, std::array<CollidableComponent *, ID_MAX>>;

  CompTuple m_components;
  size_t m_nextFreeEntity;

public:
  WorldSystem();
  ~WorldSystem();

  void Update(f32 t) override;

  void Init();

  EntityID Create(const TupleType type);
  void Destroy(const EntityID id);

  bool IsValid(const EntityID id) const { return m_entityIDs[id & INDEX_MASK] == id; }

  Entity *GetEntity(const EntityID id) { return m_entities[id]; }

  std::vector<EntityID> EntityIDsWithType(const TupleType type);

  template<typename... Ts>
  std::tuple<Ts...> GetTuple(const EntityID id) {
    return std::make_tuple(std::get<Ts...>(m_components)[id & INDEX_MASK]);
  }
};

// class ComponentManager {
//  using CompTuple = std::tuple<std::array<Physics *, ID_MAX>, std::array<Renderable *, ID_MAX>,
//      std::array<Transform *, ID_MAX>, std::array<Shaterable *, ID_MAX>, std::array<Mesh *,
//      ID_MAX>, std::array<Collidable *, ID_MAX>>;
//  CompTuple m_components;

// public:
//  ComponentManager() = default;
//  ~ComponentManager();

//  EntityID CreateEntity(EntityID type);

//  void DestroyEntity(EntityID id);

//  template<typename T>
//  T *GetComponent(const EntityID id) {
//    auto component = std::get<std::vector<T *>>(m_components);
//    u32 index = id & INDEX_MASK;
//    if (index < component.size()) {
//      return component[index];
//    }
//    return nullptr;
//  }

//  template<typename T>
//  std::vector<T *> GetComponents() {
//    return std::get<std::vector<T *>>(m_components);
//  }

// private:
//  EntityID NextID();
//};

} // namespace ecs

#endif
