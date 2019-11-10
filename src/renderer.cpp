#include "renderer.h"

#include "VertexBuffer.h"
#include "debugdraw.h"
#include "shader.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace Renderer {

static std::vector<Drawable> s_renderQueue;
static Shader *s_defaultShader = nullptr;

void AddToDrawQueue(const Drawable &drawable) {
  if (!s_defaultShader) {
    s_defaultShader = new Shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};
  }
  s_renderQueue.push_back(drawable);
}

// TODO: remove this check for a default shader
// Maybe create a shader manager
void Draw(const glm::mat4 &camera, const glm::mat4 &perspective) {
  glLineWidth(2.0f);
  for (const auto &drawable : s_renderQueue) {
    if (drawable.shader) {
      drawable.shader->Bind();
      drawable.shader->SetShaderData(drawable.shaderData);
      drawable.shader->SetUniformMat4("camera", camera);
      drawable.shader->SetUniformMat4("projection", perspective);

    } else {
      s_defaultShader->Bind();
      s_defaultShader->SetShaderData(drawable.shaderData);
      s_defaultShader->SetUniformMat4("camera", camera);
      s_defaultShader->SetUniformMat4("projection", perspective);
    }
    drawable.vertexArray->Bind();
    if (!drawable.commands.empty()) {
      glDrawElements(GL_TRIANGLES, drawable.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
    } else {
      glDrawElements(GL_LINES, drawable.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
      glDrawElements(GL_POINTS, drawable.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
    }
  }
}

void DrawImmediate(
    const glm::mat4 &camera, const glm::mat4 &perspective, const Drawable &drawable) {
  if (drawable.shader) {
    drawable.shader->Bind();
    drawable.shader->SetShaderData(drawable.shaderData);
    drawable.shader->SetUniformMat4("camera", camera);
    drawable.shader->SetUniformMat4("projection", perspective);

  } else {
    s_defaultShader->Bind();
    s_defaultShader->SetShaderData(drawable.shaderData);
    s_defaultShader->SetUniformMat4("camera", camera);
    s_defaultShader->SetUniformMat4("projection", perspective);
  }
  drawable.vertexArray->Bind();
  glDrawElements(GL_TRIANGLES, drawable.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
}

void DrawDebug(const glm::mat4 &camera, const glm::mat4 &perspective) {
  for (const auto &debug : DebugDraw::DebugMeshes()) {
    debug.shader->Bind();
    debug.shader->SetShaderData(debug.shaderData);
    debug.shader->SetUniformMat4("camera", camera);
    debug.shader->SetUniformMat4("projection", perspective);
    debug.vertexArray->Bind();
    glDrawElements(GL_LINES, debug.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
  }
}

void DisplayDraw(const Window *window) {
  glfwSwapBuffers(window->m_glWindow);
}

void Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ClearDrawQueue() {
  s_renderQueue.clear();
}

} // namespace Renderer
