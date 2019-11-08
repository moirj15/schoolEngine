#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <vector>

class Shader;
class VertexArray;
struct Window;

struct ShaderData;

namespace Renderer {

enum Command {
  DrawLine,
  DrawSolid,
  CullBack,
  DisableCull,
};

struct Drawable {
  std::vector<Command> commands;
  std::vector<ShaderData> shaderData;
  VertexArray *vertexArray;
  Shader *shader;
};

/**
 * Adds the given drawable to the draw queue.
 */
void AddToDrawQueue(const Drawable &drawable);

void Draw(const glm::mat4 &camera, const glm::mat4 &perspective);
void DrawImmediate(const glm::mat4 &camera, const glm::mat4 &perspective, const Drawable &drawable);

void DrawDebug(const glm::mat4 &camera, const glm::mat4 &perspective);

void DisplayDraw(const Window *window);

void Clear();

void ClearDrawQueue();
} // namespace Renderer

#endif // RENDERER_H
