#include "ecs.h"

#include "components.h"
#include "physicsSystem.h"
#include "rendererableSystem.h"
#include "shaterableSystem.h"
#include "CollidableSystem.h"
#include "system.h"

namespace ecs {
WorldSystem::WorldSystem() : m_nextFreeEntity(0) {
  m_systems.push_back(new PhysicsSystem(this));
  m_systems.push_back(new ShaterableSystem(this));
  m_systems.push_back(new CollidableSystem(this));
  m_systems.push_back(new RenderableSystem(this));

  for (size_t i = 0; i < m_entityIDs.size(); i++) {
    m_entityIDs[i] = FREE_ID | (u16)(i + 1);
  }
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
}

EntityID WorldSystem::Create(const TupleType type) {
  const EntityID id = NextFreeID(type);
  const size_t index = id & INDEX_MASK;
  auto &[physics, renderables, transforms, shaterables, meshes, collidables, DECLs] = m_components;
  auto *entity = new Entity;
  entity->m_id = id;
  m_entities[entity->m_id] = entity;
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
  if ((u64)type & (u64)Type::DECL) {
    DECLs[index] = new DECLComponent;
    entity->m_components.push_back(DECLs[index]);
  }

  return id;
}
void WorldSystem::Destroy(const EntityID id) {
  auto *entity = m_entities[id];
  m_entities.erase(id);
  m_entityIDs[id & INDEX_MASK] |= FREE_ID;
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

EntityID WorldSystem::NextFreeID(const TupleType type) {
  size_t index = m_nextFreeEntity;
  m_nextFreeEntity = INDEX_MASK & m_entityIDs[m_nextFreeEntity];
  m_entityIDs[index] &= (~FREE_ID);
  m_entityIDs[index] &= (~INDEX_MASK);
  m_entityIDs[index] |= (u64)type;
  return m_entityIDs[index];
}

} // namespace ecs
