#include "ecs.h"

#include "components.h"
#include "physicsSystem.h"
#include "rendererableSystem.h"
#include "shaterableSystem.h"
#include "system.h"

namespace ecs {
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

void WorldSystem::Update(f32 t) {
  for (auto *system : m_systems) {
    system->Update(t);
  }
}

void WorldSystem::Init() {
  m_systems.push_back(new PhysicsSystem(this));
  m_systems.push_back(new ShaterableSystem(this));
  m_systems.push_back(new RenderableSystem(this));
}

EntityID WorldSystem::Create(const TupleType type) {
  const EntityID id = m_entityIDs[m_nextFreeEntity];
  const size_t index = id & INDEX_MASK;
  auto &[physics, renderables, transforms, shaterables, meshes, collidables] = m_components;
  auto *entity = new Entity;
  entity->m_id = id;
  if ((u64)type & (u64)Type::Renderable) {
    renderables[index] = new RenderableComponent;
    entity->m_components.push_back(renderables[index]);
  }
  if ((u64)type & (u64)Type::Physics) {
    physics[index] = new PhysicsComponent;
    entity->m_components.push_back(physics[index]);
  }
  if ((u64)type & (u64)Type::Transform) {
    transforms[index] = new TransformComponent;
    entity->m_components.push_back(transforms[index]);
  }
  if ((u64)type & (u64)Type::Shaterable) {
    shaterables[index] = new ShaterableComponent;
    entity->m_components.push_back(shaterables[index]);
  }
  if ((u64)type & (u64)Type::Mesh) {
    meshes[index] = new MeshComponent;
    entity->m_components.push_back(meshes[index]);
  }
  if ((u64)type & (u64)Type::Collidable) {
    collidables[index] = new CollidableComponent;
    entity->m_components.push_back(collidables[index]);
  }

  return id;
}
void WorldSystem::Destroy(const EntityID id) {
  auto *entity = m_entities[id];
  m_entities.erase(id);
  m_entityIDs[id & INDEX_MASK] = FREE_ID;
  delete (entity);
}

std::vector<EntityID> WorldSystem::EntityIDsWithType(const TupleType type) {
  std::vector<EntityID> ids;
  for (const auto &pair : m_entities) {
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
} // namespace ecs

