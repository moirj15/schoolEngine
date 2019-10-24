#include "shaterableManager.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/normal.hpp>

void ShaterableManager::Simulate(f32 prevTimeStep, f32 currTimeStep) {
  auto physicsComponents = m_componentManager->GetComponents<ECS::Physics>();
  auto transformsComponents =
      m_componentManager->GetComponents<ECS::Transform>();
  auto shaterablesComponents =
      m_componentManager->GetComponents<ECS::Shaterable>();
  auto meshComponents = m_componentManager->GetComponents<ECS::Mesh>();
  for (size_t i = 0; i < physicsComponents.size(); i++) {
    auto *physics = physicsComponents[i];
    auto *transform = transformsComponents[i];
    auto *shaterable = shaterablesComponents[i];
    auto *mesh = meshComponents[i];
    if (physics && transform && shaterable && mesh) {
      if (!shaterable->hasShatered) {
        shaterable->hasShatered = true;
        auto triangles = ProcessTriangles(mesh);
        AddTrianglesToECS(triangles);
      }
    }
  }
}

std::vector<Triangle> ShaterableManager::ProcessTriangles(ECS::Mesh *mesh) {
  std::vector<Triangle> triangles;
  const auto &c = mesh->connections;
  const auto &v = mesh->vertecies;
  for (size_t i = 0; i < mesh->connections.size() - 3; i += 3) {
    triangles.push_back({{v[c[i] * 3], v[(c[i] * 3) + 1], v[(c[i] * 3) + 2]},
        {v[c[i + 1] * 3], v[(c[i + 1] * 3) + 1], v[(c[i + 1] * 3) + 2]},
        {v[c[i + 2] * 3], v[(c[i + 2] * 3) + 1], v[(c[i + 2] * 3) + 2]}});
  }
  return triangles;
}

void ShaterableManager::AddTrianglesToECS(
    const std::vector<Triangle> &triangles) {
  for (const auto &triangle : triangles) {
    u32 type = static_cast<u32>(ECS::Type::Renderable)
               | static_cast<u32>(ECS::Type::Physics)
               | static_cast<u32>(ECS::Type::Transform);
    ECS::EntityID id = m_componentManager->CreateEntity(type);
    // TODO: do something with physics
    auto *physics = m_componentManager->GetComponent<ECS::Physics>(id);
    auto *renderable = m_componentManager->GetComponent<ECS::Renderable>(id);
    auto *transform = m_componentManager->GetComponent<ECS::Transform>(id);

    renderable->vertexArray = new VertexArray();
    u32 indecies[] = {0, 1, 2};
    f32 verts[] = {
        triangle.p0.x,
        triangle.p0.y,
        triangle.p0.z,
        triangle.p1.x,
        triangle.p1.y,
        triangle.p1.z,
        triangle.p2.x,
        triangle.p2.y,
        triangle.p2.z,
    };
    renderable->vertexArray->AddIndexBuffer(new IndexBuffer{indecies, 3});
    renderable->vertexArray->AddVertexBuffer(
        new VertexBuffer{verts, 9, {{"boxVerts", 3, 0, 0, GL_FLOAT}}});

    transform->position = {0.0f, 0.0f, -1.0f};
    transform->rotation = {};
    physics->velocity =
        glm::triangleNormal(triangle.p0, triangle.p1, triangle.p2)
        + triangle.p1;
  }
}