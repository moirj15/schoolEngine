#include "renderer.h"

#include "../window.h"
#include "VertexBuffer.h"
#include "debugdraw.h"
#include "shader.h"
#include "shaderLibrary.h"

#include <GLFW/glfw3.h>
#include <vector>

namespace renderer {

static std::vector<Drawable> s_renderQueue;
static Shader *s_defaultShader = nullptr;
static ShaderLibrary s_shaderLibrary;

void Init() {
  s_shaderLibrary.Add({"../shaders/shader.vert", "../shaders/shader.frag"});
}

void AddToDrawQueue(const Drawable &drawable) {
  //  if (!s_defaultShader) {
  //    s_defaultShader = new Shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};
  //  }
  s_renderQueue.push_back(drawable);
}

// TODO: remove this check for a default shader
// Maybe create a shader manager
void Draw(const glm::mat4 &camera, const glm::mat4 &perspective) {
  glLineWidth(2.0f);
  for (const auto &drawable : s_renderQueue) {
    auto *shader = s_shaderLibrary.GetProgram(drawable.m_name);
    shader->Bind();
    shader->SetShaderData(drawable.m_shaderData);
    shader->SetUniformMat4("camera", camera);
    shader->SetUniformMat4("projection", perspective);
    drawable.m_vertexArray->Bind();

    switch (drawable.m_commands.back()) {
    case Command::DrawSolid:
      glDrawElements(
          GL_TRIANGLES, drawable.m_vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
      break;
    case Command::DrawLine:
      break;
    case Command::DrawPoints:
      glDrawElements(GL_POINTS, drawable.m_vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
      break;
    }
  }
}

void DrawImmediate(
    const glm::mat4 &camera, const glm::mat4 &perspective, const Drawable &drawable) {
  printf("REFRACTOR_ME\n");
  //  if (drawable.shader) {
  //    drawable.shader->Bind();
  //    drawable.shader->SetShaderData(drawable.m_shaderData);
  //    drawable.shader->SetUniformMat4("camera", camera);
  //    drawable.shader->SetUniformMat4("projection", perspective);
  //
  //  } else {
  //    s_defaultShader->Bind();
  //    s_defaultShader->SetShaderData(drawable.m_shaderData);
  //    s_defaultShader->SetUniformMat4("camera", camera);
  //    s_defaultShader->SetUniformMat4("projection", perspective);
  //  }
  //  drawable.m_vertexArray->Bind();
  //  glDrawElements(GL_TRIANGLES, drawable.m_vertexArray->IndexCount(), GL_UNSIGNED_INT, (void
  //  *)0);
}

void DrawDebug(const glm::mat4 &camera, const glm::mat4 &perspective) {
  printf("REFRACTOR_ME\n");
  //  for (const auto &debug : DebugDraw::DebugMeshes()) {
  //    debug.shader->Bind();
  //    debug.shader->SetShaderData(debug.shaderData);
  //    debug.shader->SetUniformMat4("camera", camera);
  //    debug.shader->SetUniformMat4("projection", perspective);
  //    debug.m_vertexArray->Bind();
  //    glDrawElements(GL_LINES, debug.m_vertexArray->IndexCount(), GL_UNSIGNED_INT, (void *)0);
  //  }
}

void DisplayDraw(const Window *window) {
  glfwSwapBuffers(window->m_glWindow);
}

void Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ClearDrawQueue() {
  for (auto &drawable : s_renderQueue) {
    delete(drawable.m_vertexArray);
  }
  s_renderQueue.clear();
}

} // namespace renderer
