//
// Created by Jimmy on 12/15/2019.
//

#include "CollidableSystem.h"

#include "components.h"
#include "ecs.h"

namespace ecs {

CollidableSystem::CollidableSystem(WorldSystem *world) : m_world(world) {
}

void CollidableSystem::Update(f32 t) {
  auto tuples = GetCollidableTuples();
  for (size_t i = 0; i < tuples.size(); i++) {
    for (size_t j = 0; j < tuples.size(); j++) {
      if (i == j) {
        continue;
      }
      auto *collidableA = tuples[i].m_collidable;
      auto *DECLA = tuples[i].m_DECL;
      auto *transformA = tuples[i].m_transform;
      auto *collidableB = tuples[j].m_collidable;
      auto *DECLB = tuples[j].m_DECL;
      auto *transformB = tuples[j].m_transform;
      f32 length = glm::length(transformA->m_position - transformB->m_position);
      if ((length < collidableA->m_boundingSphereRadius)
          || (length < collidableB->m_boundingSphereRadius)) {
        collidableA->m_hasCollided = true;
        collidableB->m_hasCollided = true;
        UpdateDecl(&tuples[i], transformB->m_position);
        UpdateDecl(&tuples[j], transformA->m_position);
      }
    }
  }
}

std::vector<CollidableTuple> CollidableSystem::GetCollidableTuples() {
  std::vector<CollidableTuple> ret;
  auto ids = m_world->EntityIDsWithType(TupleType::CollidableTuple);
  for (auto id : ids) {
    if (m_world->IsValid(id)) {
      auto [collidable, DECL, transform, mesh] =
          m_world->GetTuple<CollidableComponent *, DECLComponent *, TransformComponent *, MeshComponent*>(id);
      ret.push_back({collidable, DECL, transform, mesh});
    }
  }

  return ret;
}

void CollidableSystem::UpdateDecl(CollidableTuple *tuple, const glm::vec3 &position) {
  // Try the naive way first to get the other systems working
  auto *DECL= tuple->m_DECL;
  auto *collidable = tuple->m_collidable;
  auto *mesh = tuple->m_mesh;
  for (size_t i = 0; i < DECL->m_triangles.size(); i++) {
    auto *triangle = DECL->m_triangles[i];
    for (auto &edge : triangle->m_edges) {
      u32 index = edge.start;
      glm::vec3 point(mesh->m_vertecies[index], mesh->m_vertecies[index + 1],mesh->m_vertecies[index + 2]);
      if (glm::length(point - position) < collidable->m_boundingSphereRadius) {
        collidable->m_collidedTriangles.insert(i);
      }
    }
  }
}

} // namespace ecs
