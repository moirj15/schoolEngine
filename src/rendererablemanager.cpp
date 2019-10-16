#include "rendererablemanager.h"

#include "renderer.h"

void RendererableManager::DrawComponents() {
  auto renderables = m_componentManager->GetComponents<ECS::Renderable>();
  auto transforms = m_componentManager->GetComponents<ECS::Transform>();
  for (size_t i = 0; i < renderables.size(); i++) {
    auto *drawable = renderables[i];
    auto *transform = transforms[i];
    if (drawable && transform) {}
  }
}
