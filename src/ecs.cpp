#include "ecs.h"

namespace ECS {

std::vector<Physics *> s_physicsEntities;

static EntityID NextID() {
  static EntityID id = 0;
  id++;
  return id;
}

void Init() {
  // TODO
}

void DeInit() {
  // TODO
}

EntityID CreateEntity(Type type) {
  EntityID id = NextID();
  if (type == Type::Renderable) {
    id |= static_cast<u32>(Type::Renderable);

  } else if (type == Type::Physics) {
    id |= static_cast<u32>(Type::Physics);
    s_physicsEntities.emplace_back(new Physics{id});
  }
  return id;
}

// TODO: Consider templating this
std::optional<Physics *> GetPhysicsComponent(EntityID id) {
  if (((id && 0xffff) < s_physicsEntities.size()) && s_physicsEntities[id]) {
    return s_physicsEntities[id];
  }
  return {};
}

std::vector<Physics *> GetPhysics() {
  return s_physicsEntities;
}
} // namespace ECS