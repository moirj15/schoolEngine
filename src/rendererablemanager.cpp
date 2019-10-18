#include "rendererablemanager.h"

#include "renderer.h"
glm::mat4 perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f);
glm::mat4 camera = glm::lookAt(glm::vec3{0.0f, 0.0f, 0.0f},
    glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

void RendererableManager::DrawComponents() {
  auto renderables = m_componentManager->GetComponents<ECS::Renderable>();
  auto transforms = m_componentManager->GetComponents<ECS::Transform>();
  for (size_t i = 0; i < renderables.size(); i++) {
    auto *drawable = renderables[i];
    auto *transform = transforms[i];
    if (drawable && transform && i != 1) {
      // TODO: FIX THIS GOD AWFUL MESS
      auto copy = *drawable;
      auto transformMat = glm::translate(glm::mat4(1.0f), transform->position)
                          * glm::mat4(transform->rotation);
      copy.shaderData.push_back({"transform", transformMat});

      Renderer::AddToDrawQueue(
          {copy.commands, copy.shaderData, copy.vertexArray, copy.shader});
    }
  }
}
