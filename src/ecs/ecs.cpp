#include "ecs.h"

namespace ECS {

WorldSystem::WorldSystem() {
}
WorldSystem::~WorldSystem() {
  for (auto *system : m_systems) {
    if (system) {
      delete (system);
    }
  }
  for (auto &pair : m_entities) {
    auto *entity = pair.second;
    if (entity) {
      delete (entity);
    }
  }
}

EntityID WorldSystem::Create(const TupleType type) {
}
void WorldSystem::Destroy(const EntityID id) {
  auto *entity = m_entities[id];
  m_entities.erase(id);
  m_entityIDs[id & INDEX_MASK] = FREE_ID;
  delete (entity);
}

std::vector<EntityID> WorldSystem::EntityIDsWithType(const TupleType type) {
  std::vector<EntityID> ids;
  for (auto pair : m_entities) {
    auto id = pair.first;
    if ((id & (u64)type) == (u64)type) {
      ids.push_back(id);
    }
  }
  return ids;
}

// ComponentManager::~ComponentManager() {
//  auto [physics, renderables, transforms, shaterables, meshes, collidables] = m_components;
//  for (u32 i = 0; i < physics.size(); i++) {
//    delete (physics[i]);
//    delete (renderables[i]);
//    delete (transforms[i]);
//  }
//}
//
// EntityID ComponentManager::CreateEntity(EntityID type) {
//  EntityID id = NextID();
//  auto &[physics, renderables, transforms, shaterables, meshes, collidables] = m_components;
//  if (type & static_cast<EntityID>(Type::Renderable)) {
//    renderables[id] = new Renderable;
//  }
//  if (type & static_cast<EntityID>(Type::Physics)) {
//    physics[id] = new Physics;
//  }
//  if (type & static_cast<EntityID>(Type::Transform)) {
//    transforms[id] = new Transform;
//  }
//  if (type & static_cast<EntityID>(Type::Shaterable)) {
//    shaterables[id] = new Shaterable;
//  }
//  if (type & static_cast<EntityID>(Type::Mesh)) {
//    meshes[id] = new Mesh;
//  }
//  if (type & static_cast<EntityID>(Type::Collidable)) {
//    collidables[id] = new Collidable;
//  }
//  id |= type;
//  return id;
//}
//
// void ComponentManager::DestroyEntity(EntityID id) {
//  auto [physics, renderables, transforms, shaterables, meshes, collidables] = m_components;
//  u32 index = id & 0x0000ffff;
//  if (id & static_cast<u32>(Type::Physics)) {
//    delete (physics[index]);
//    physics[index] = nullptr;
//  }
//  if (id & static_cast<u32>(Type::Renderable)) {
//    delete (renderables[index]);
//    renderables[index] = nullptr;
//  }
//  if (id & static_cast<u32>(Type::Transform)) {
//    delete (transforms[index]);
//    transforms[index] = nullptr;
//  }
//}
//
// EntityID ComponentManager::NextID() {
//  static EntityID id = 0;
//  id++;
//  return id;
//}
} // namespace ECS
