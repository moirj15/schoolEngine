#include "ecs.h"

namespace ECS {

void Init() {
  // TODO
}

void DeInit() {
  // TODO
}

EntityID ComponentManager::CreateEntity(u32 type) {
  EntityID id = NextID();
  auto [physics, renderables, transforms] = m_components;
  if (type & static_cast<u32>(Type::Renderable)) {
    renderables[id] = new Renderable(id);
  }
  if (type & static_cast<u32>(Type::Physics)) { physics[id] = new Physics(id); }
  if (type & static_cast<u32>(Type::Transform)) {
    transforms[id] = new Transform(id);
  }
  id |= type;
  return id;
}

// TODO: Consider templating this
std::optional<Physics *> GetPhysicsComponent(EntityID id) {
  u32 index = id && 0x0000ffff;
  u32 count = (id && 0x00ff0000) >> 16;
  if ((index < s_physicsEntities.size()) && s_physicsEntities[index]) {
    return s_physicsEntities[id];
  }
  return {};
}
EntityID ComponentManager::NextID() {
  static EntityID id = 0;
  id++;
  return id;
}
} // namespace ECS