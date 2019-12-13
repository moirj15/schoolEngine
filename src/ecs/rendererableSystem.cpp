#include "rendererableSystem.h"

#include "../renderer/VertexBuffer.h"
#include "../renderer/renderer.h"
#include "../renderer/shader.h"
#include "components.h"
#include "ecs.h"

#include <glm/gtc/matrix_transform.hpp>
glm::mat4 perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f);
glm::mat4 camera = glm::lookAt(
    glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

// void RendererableManager::DrawComponents() {
//  auto renderables = m_componentManager->GetComponents<ecs::Renderable>();
//  auto transforms = m_componentManager->GetComponents<ecs::Transform>();
//  for (Size i = 0; i < renderables.size(); i++) {
//    auto *drawable = renderables[i];
//    auto *transform = transforms[i];
//    if (drawable && transform && i != 1) {
//      // TODO: FIX THIS GOD AWFUL MESS
//      auto copy = *drawable;
//      auto transformMat =
//          glm::translate(glm::mat4(1.0f), transform->position) * glm::mat4(transform->rotation);
//      //      copy.shaderData.push_back({"transform", transformMat});

//      //      Renderer::AddToDrawQueue(
//      //          {copy.commands, copy.shaderData, copy.vertexArray, copy.shader});
//    }
//  }
//}
namespace ecs {

const glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
const glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
const glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

RenderableSystem::RenderableSystem(WorldSystem *world) : m_world(world) {
  for (auto &tuple : GetRenderableTuple()) {
    auto *mesh = tuple.m_mesh;
    auto *renderable = tuple.m_renderable;
    auto *transform = tuple.m_transform;
    // TODO: figure out how to get around constantly allocating stuff.
    VertexArray *va;
    va->AddVertexBuffer(new VertexBuffer(
        mesh->m_vertecies.data(), mesh->m_vertecies.size(), {{"name", 3, 0, 0, GL_FLOAT}}));
    va->AddIndexBuffer(new IndexBuffer(mesh->m_connections.data(), mesh->m_connections.size()));

    std::vector<ShaderData> shaderData;
    auto translate = glm::translate(glm::mat4(1.0f), transform->m_position);
    auto rotation = glm::rotate(glm::mat4(1.0f), transform->m_rotation.x, X_AXIS)
                    * glm::rotate(glm::mat4(1.0f), transform->m_rotation.y, Y_AXIS)
                    * glm::rotate(glm::mat4(1.0f), transform->m_rotation.z, Z_AXIS);
    auto transformMat = translate * rotation;
    shaderData.emplace_back("transform", transformMat);

    ::Renderer::Drawable drawable = {renderable->m_commands, shaderData, va, renderable->m_shader};
    ::Renderer::AddToDrawQueue(drawable)
  }
}

RenderableSystem::~RenderableSystem() {
}

void RenderableSystem::Update(f32 t) {
}
std::vector<RenderableTuple> RenderableSystem::GetRenderableTuple() {
  std::vector<RenderableTuple> ret;
  auto ids = m_world->EntityIDsWithType(TupleType::RenderableTuple);
  for (auto id : ids) {
    if (m_world->IsValid(id)) {
      auto [renderable, transform, mesh] =
          m_world->GetTuple<RenderableComponent *, TransformComponent *, MeshComponent *>(id);
      ret.emplace_back(renderable, transform, mesh);
    }
  }

  return ret;
}
} // namespace ecs
