#pragma once
#include <glm/mat4x4.hpp>
#include <vector>
#include <string>

class VertexArray;
struct Window;


namespace renderer {
struct ShaderData;

enum class Command {
  DrawLine,
  DrawSolid,
  DrawPoints,
  CullBack,
  DisableCull,
};

struct Drawable {
  std::vector<Command> m_commands;
  std::vector<ShaderData> m_shaderData;
  VertexArray *m_vertexArray;
  std::string m_name;
};

void Init();

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
} // namespace renderer
