#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <glm/glm.hpp>

class Shader;
class VertexArray;

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
    glm::mat4 transforms;
};

void AddToDrawQueue(Drawable &drawable);

void Draw();

void DrawDebug();

void DisplayDraw();

void Clear();

}

#endif // RENDERER_H
