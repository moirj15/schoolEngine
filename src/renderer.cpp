#include "renderer.h"
#include "shader.h"
#include "VertexBuffer.h"
#include "debugdraw.h"
#include "window.h"
#include <vector>

namespace Renderer {

static std::vector<Drawable> s_renderQueue;

void AddToDrawQueue(const Drawable &drawable) {
    s_renderQueue.push_back(drawable);
}

void Draw(const glm::mat4 &camera, const glm::mat4 &perspective) {
    for (const auto &drawable : s_renderQueue) {
        drawable.shader->Bind();
        drawable.shader->SetShaderData(drawable.shaderData);
        drawable.shader->SetUniformMat4("camera", camera);
        drawable.shader->SetUniformMat4("projection", perspective);
        drawable.vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, drawable.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

void DrawDebug(const glm::mat4 &camera, const glm::mat4 &perspective) {
    for (const auto &debug : DebugDraw::DebugMeshes()) {
        debug.shader->Bind();
        debug.shader->SetShaderData(debug.shaderData);
        debug.shader->SetUniformMat4("camera", camera);
        debug.shader->SetUniformMat4("projection", perspective);
        debug.vertexArray->Bind();
        glDrawElements(GL_LINES, debug.vertexArray->IndexCount(), GL_UNSIGNED_INT, (void*)0);
    }
}

void DisplayDraw(const Window *window) {
    glfwSwapBuffers(window->m_glWindow);
}

void Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}
