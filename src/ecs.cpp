#include "ecs.h"

namespace ECS {

ComponentManager::~ComponentManager() {
  auto [physics, renderables, transforms, shaterables, meshes, collidables] =
      m_components;
  for (u32 i = 0; i < physics.size(); i++) {
    delete (physics[i]);
    delete (renderables[i]);
    delete (transforms[i]);
  }
}

EntityID ComponentManager::CreateEntity(u32 type) {
  EntityID id = NextID();
  auto [physics, renderables, transforms, shaterables, meshes, collidables] =
      m_components;
  if (type & static_cast<u32>(Type::Renderable)) {
    renderables[id] = new Renderable;
  }
  if (type & static_cast<u32>(Type::Physics)) { physics[id] = new Physics; }
  if (type & static_cast<u32>(Type::Transform)) {
    transforms[id] = new Transform;
  }
  id |= type;
  return id;
}

void ComponentManager::DestroyEntity(EntityID id) {
  auto [physics, renderables, transforms, shaterables, meshes, collidables] =
      m_components;
  u32 index = id & 0x0000ffff;
  if (id & static_cast<u32>(Type::Physics)) {
    delete (physics[id]);
    physics[id] = nullptr;
  }
  if (id & static_cast<u32>(Type::Renderable)) {
    delete (renderables[id]);
    renderables[id] = nullptr;
  }
  if (id & static_cast<u32>(Type::Transform)) {
    delete (transforms[id]);
    transforms[id] = nullptr;
  }
}

EntityID ComponentManager::NextID() {
  static EntityID id = 0;
  id++;
  return id;
}
} // namespace ECS
