#include "renderer.h"
#include "shader.h"
#include "VertexBuffer.h"
#include <vector>

namespace Renderer {

static std::vector<Drawable> s_renderQueue;

void AddToDrawQueue(Drawable &drawable) {
    s_renderQueue.emplace_back(drawable);
}

void Draw() {
    for (const auto &drawable : s_renderQueue) {
        drawable.shader->Bind();
        drawable.vertexArray->Bind();
    }
}

void DrawDebug() {

}

void DisplayDraw() {

}

void Clear() {

}

}
