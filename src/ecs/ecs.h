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
namespace renderer {
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

const u64 COUNT_MASK = 0x0000000000ff0000;
const u64 INDEX_MASK = 0x000000000000ffff;
const u16 ID_MAX = 0xffff;
const u64 FREE_ID = 0x8000000000000000;

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
  ShaterableTuple =
      (u64)Type::Physics | (u64)Type::Transform | (u64)Type::Shaterable | (u64)Type::Collidable,
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

  bool IsValid(const EntityID id) const {
    return m_entityIDs[id & INDEX_MASK] == id;
  }

  Entity *GetEntity(const EntityID id) { return m_entities[id]; }

  std::vector<EntityID> EntityIDsWithType(const TupleType type);

  template<typename... Ts>
  auto GetTuple(const EntityID id) {
    return std::make_tuple(std::get<std::array<Ts, ID_MAX>>(m_components)[id & INDEX_MASK]...);
  }
private:
  EntityID NextFreeID(const TupleType type);
};

} // namespace ecs

#endif
