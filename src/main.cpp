#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/transform.hpp>

#include "window.h"
#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

constexpr s32 width = 1920;
constexpr s32 height = 1080;

void InitGL() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    InitGL();
    Window window{width, height};
    Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};
    shader.Bind();
    shader.SetUniformMat4("projection", glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f));
    shader.SetUniformMat4("transform", glm::mat4(1.0f));
    shader.SetUniformMat4("camera", glm::mat4());

    //glfwSetInputMode(window.m_glWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    f64 lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window.m_glWindow)) {
        glfwPollEvents();

        f64 currentTime = glfwGetTime();
        f64 delta = (currentTime - lastTime) * 1000.0;

        lastTime = glfwGetTime();
        glfwSwapBuffers(window.m_glWindow);
        //printf("%f\n", 1000.0 / delta);
    }
    return 0;
}
