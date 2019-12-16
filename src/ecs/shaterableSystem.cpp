#include "shaterableSystem.h"

#include "../renderer/VertexBuffer.h"
#include "../renderer/renderer.h"
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
      collidable->m_hasCollided = false;
    }
  }
}

std::vector<ShaterableTuple> ShaterableSystem::GetShaterableTuple() {
  std::vector<ShaterableTuple> ret;
  auto ids = m_world->EntityIDsWithType(TupleType::ShaterableTuple);
  for (auto id : ids) {
    if (m_world->IsValid(id)) {
      auto [physics, mesh, shaterable, collidable, DECL, transform] =
          m_world->GetTuple<PhysicsComponent *, MeshComponent *, ShaterableComponent *,
              CollidableComponent *, DECLComponent *, TransformComponent *>(id);
      ret.push_back({id, physics, mesh, shaterable, collidable, DECL, transform});
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
  DECL->m_triangles = nonShattered;
  if (shattered.empty()) {
    return;
  }
  for (auto *shatteredTri : shattered) {
    auto id = m_world->Create(
        (TupleType)((u64)TupleType::RenderableTuple | (u64)TupleType::PhysicsTuple));
    auto [renderable, mesh, transform, physics] = m_world->GetTuple<RenderableComponent *,
        MeshComponent *, TransformComponent *, PhysicsComponent *>(id);
    // Populate renderable component
    renderable->m_texture = "";
    renderable->m_shader = "shader";
    renderable->m_commands = {::renderer::Command::DrawSolid};

    // Populate mesh component
    mesh->m_connections = {0, 1, 2};
    std::vector<glm::vec3> points;
    for (auto edge : shatteredTri->m_edges) {
      u32 c = edge.start;
      mesh->m_vertecies.push_back(tuple->m_mesh->m_vertecies[c]);
      mesh->m_vertecies.push_back(tuple->m_mesh->m_vertecies[c + 1]);
      mesh->m_vertecies.push_back(tuple->m_mesh->m_vertecies[c + 2]);
      points.push_back({tuple->m_mesh->m_vertecies[c], tuple->m_mesh->m_vertecies[c + 1],
          tuple->m_mesh->m_vertecies[c + 2]});
    }
    glm::vec3 center(0.0f, 0.0f, 0.0f);
    for (const auto &p : points) {
      center += p;
    }
    center /= 3.0f;

    // Populate transform component
//    transform->m_rotation = tuple->m_transform->m_rotation;
    transform->m_position = tuple->m_transform->m_position;// + center;

    // Populate physics component
    // TODO: make better
    auto [collidedPhysics] = m_world->GetTuple<PhysicsComponent*>(collidable->m_id);
    physics->m_velocity = collidedPhysics->m_velocity + glm::triangleNormal(points[0], points[1], points[2]);//tuple->m_physics->m_velocity;
  }
  std::vector<u32> newConnections;
  std::vector<f32> newPoints;
  for (auto *nonShatteredTrie : nonShattered) {
    // TODO: modify the nonshattered mesh to remove the shattered triangles.
    for (auto &edge : nonShatteredTrie->m_edges) {
      u32 c = edge.start;
      newConnections.push_back(newConnections.size());
      newPoints.push_back(mesh->m_vertecies[c]);
      newPoints.push_back(mesh->m_vertecies[c + 1]);
      newPoints.push_back(mesh->m_vertecies[c + 2]);
    }
  }
  mesh->m_vertecies  = newPoints;
  mesh->m_connections = newConnections;
  if (nonShattered.empty()) {
    m_world->Destroy(tuple->m_id);
  }
}

} // namespace ecs
