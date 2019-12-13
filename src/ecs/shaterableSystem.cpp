#include "shaterableSystem.h"

#include "../renderer/VertexBuffer.h"
#include "ecs.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>

namespace ecs {

ShaterableSystem::ShaterableSystem(WorldSystem *world) : m_world(world) {
}
ShaterableSystem::~ShaterableSystem() {
}

void ShaterableSystem::Update(f32 t) {
}

} // namespace ecs

// void ShaterableManager::Simulate(f32 prevTimeStep, f32 currTimeStep) {
//  auto physicsComponents = m_componentManager->GetComponents<ecs::Physics>();
//  auto transformsComponents = m_componentManager->GetComponents<ecs::Transform>();
//  auto shaterablesComponents = m_componentManager->GetComponents<ecs::Shaterable>();
//  auto meshComponents = m_componentManager->GetComponents<ecs::Mesh>();
//  for (Size i = 0; i < physicsComponents.size(); i++) {
//    auto *physics = physicsComponents[i];
//    auto *transform = transformsComponents[i];
//    auto *shaterable = shaterablesComponents[i];
//    auto *mesh = meshComponents[i];
//    if (physics && transform && shaterable && mesh) {
//      if (!shaterable->hasShatered) {
//        shaterable->hasShatered = true;
//        auto triangles = ProcessTriangles(mesh);
//        AddTrianglesToECS(triangles);
//      }
//    }
//  }
//}
//
// std::vector<Triangle> ShaterableManager::ProcessTriangles(ecs::Mesh *mesh) {
//  std::vector<Triangle> triangles;
//  const auto &c = mesh->connections;
//  const auto &v = mesh->vertecies;
//  for (Size i = 0; i < mesh->connections.size() - 3; i += 3) {
//    triangles.push_back({{v[c[i] * 3], v[(c[i] * 3) + 1], v[(c[i] * 3) + 2]},
//        {v[c[i + 1] * 3], v[(c[i + 1] * 3) + 1], v[(c[i + 1] * 3) + 2]},
//        {v[c[i + 2] * 3], v[(c[i + 2] * 3) + 1], v[(c[i + 2] * 3) + 2]}});
//  }
//  return triangles;
//}
//
// void ShaterableManager::AddTrianglesToECS(const std::vector<Triangle> &triangles) {
//  for (const auto &triangle : triangles) {
//    u32 type = static_cast<u32>(ecs::Type::Renderable) | static_cast<u32>(ecs::Type::Physics)
//               | static_cast<u32>(ecs::Type::Transform);
//    ecs::EntityID id = m_componentManager->CreateEntity(type);
//    // TODO: do something with physics
//    auto *physics = m_componentManager->GetComponent<ecs::Physics>(id);
//    auto *renderable = m_componentManager->GetComponent<ecs::Renderable>(id);
//    auto *transform = m_componentManager->GetComponent<ecs::Transform>(id);
//
//    renderable->vertexArray = new VertexArray();
//    u32 indecies[] = {0, 1, 2};
//    f32 verts[] = {
//        triangle.p0.x,
//        triangle.p0.y,
//        triangle.p0.z,
//        triangle.p1.x,
//        triangle.p1.y,
//        triangle.p1.z,
//        triangle.p2.x,
//        triangle.p2.y,
//        triangle.p2.z,
//    };
//    renderable->vertexArray->AddIndexBuffer(new IndexBuffer{indecies, 3});
//    renderable->vertexArray->AddVertexBuffer(
//        new VertexBuffer{verts, 9, {{"boxVerts", 3, 0, 0, GL_FLOAT}}});
//
//    transform->position = {0.0f, 0.0f, -1.0f};
//    transform->rotation = {};
//    physics->velocity = glm::triangleNormal(triangle.p0, triangle.p1, triangle.p2) + triangle.p1;
//  }
//}
