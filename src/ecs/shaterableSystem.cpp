#include "shaterableSystem.h"

#include "../renderer/VertexBuffer.h"
#include "components.h"
#include "ecs.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>

namespace ecs {

ShaterableSystem::ShaterableSystem(WorldSystem *world) : m_world(world) {
}

void ShaterableSystem::Update(f32 t) {
  for (auto &tuple : GetShaterableTuple()) {
    auto collidable = tuple.m_collidable;
    if (collidable->m_hasCollided) {
      DecomposeShaterable(&tuple);
    }
  }
}

std::vector<ShaterableTuple> ShaterableSystem::GetShaterableTuple() {
  std::vector<ShaterableTuple> ret;
  auto ids = m_world->EntityIDsWithType(TupleType::ShaterableTuple);
  for (auto id : ids) {
    if (m_world->IsValid(id)) {
      auto [physics, mesh, shaterable, collidable, DECL] = m_world->GetTuple<PhysicsComponent *,
          MeshComponent *, ShaterableComponent *, CollidableComponent *, DECLComponent *>(id);
      ret.push_back({id, physics, mesh, shaterable, collidable, DECL});
    }
  }

  return ret;
}
void ShaterableSystem::DecomposeShaterable(ShaterableTuple *tuple) {
  auto *collidable = tuple->m_collidable;
  auto *DECL = tuple->m_DECL;
  auto *mesh = tuple->m_mesh;
  std::vector<Triangle *> nonShattered;
  std::vector<Triangle *> shattered;
  for (size_t i = 0; i < DECL->m_triangles.size(); i++) {
    if (collidable->m_collidedTriangles.find(i) != collidable->m_collidedTriangles.end()) {
      shattered.push_back(DECL->m_triangles[i]);
    } else {
      nonShattered.push_back(DECL->m_triangles[i]);
    }
  }
  if (nonShattered.empty()) {
    m_world->Destroy(tuple->m_id);
  }

}

} // namespace ecs

