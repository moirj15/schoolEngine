#pragma once
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

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

/**
 * Draws the current contents of the draw queue using the givne camera and perspective.
 * @param camera: The camera.
 * @param perspective: the perspective.
 */
void Draw(const glm::mat4 &camera, const glm::mat4 &perspective);

/**
 * Immediatly draws the given drawable.
 * @param camera: The camera.
 * @param perspective: the persepctive.
 * @param drawable: The drawable.
 */
void DrawImmediate(const glm::mat4 &camera, const glm::mat4 &perspective, const Drawable &drawable);

/// For drawing debug information.
void DrawDebug(const glm::mat4 &camera, const glm::mat4 &perspective);

/// Displays the draw to the given window.
void DisplayDraw(const Window *window);

/// clears the screen
void Clear();

/// Clears the draw queue.
void ClearDrawQueue();
} // namespace renderer
