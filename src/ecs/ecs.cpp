#include "ecs.h"

namespace ECS {

ComponentManager::~ComponentManager() {
  auto [physics, renderables, transforms, shaterables, meshes, collidables,
      skeletons] = m_components;
  for (u32 i = 0; i < physics.size(); i++) {
    delete (physics[i]);
    delete (renderables[i]);
    delete (transforms[i]);
  }
}

EntityID ComponentManager::CreateEntity(EntityID type) {
  EntityID id = NextID();
  auto &[physics, renderables, transforms, shaterables, meshes, collidables,
      skeletons] = m_components;
  if (type & static_cast<EntityID>(Type::Renderable)) {
    renderables[id] = new Renderable;
  }
  if (type & static_cast<EntityID>(Type::Physics)) {
    physics[id] = new Physics;
  }
  if (type & static_cast<EntityID>(Type::Transform)) {
    transforms[id] = new Transform;
  }
  if (type & static_cast<EntityID>(Type::Shaterable)) {
    shaterables[id] = new Shaterable;
  }
  if (type & static_cast<EntityID>(Type::Mesh)) { meshes[id] = new Mesh; }
  if (type & static_cast<EntityID>(Type::Collidable)) {
    collidables[id] = new Collidable;
  }
  if (type & static_cast<EntityID>(Type::Skeleton)) {
    skeletons[id] = new Skeleton;
  }
  id |= type;
  return id;
}

void ComponentManager::DestroyEntity(EntityID id) {
  auto [physics, renderables, transforms, shaterables, meshes, collidables,
      skeletons] = m_components;
  u32 index = id & 0x0000ffff;
  if (id & static_cast<u32>(Type::Physics)) {
    delete (physics[index]);
    physics[index] = nullptr;
  }
  if (id & static_cast<u32>(Type::Renderable)) {
    delete (renderables[index]);
    renderables[index] = nullptr;
  }
  if (id & static_cast<u32>(Type::Transform)) {
    delete (transforms[index]);
    transforms[index] = nullptr;
  }
}

EntityID ComponentManager::NextID() {
  static EntityID id = 0;
  id++;
  return id;
}
} // namespace ECS
