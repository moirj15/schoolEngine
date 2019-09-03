#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#if 0
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/transform.hpp>

#include "window.h"
#include "shader.h"

#include "VertexBuffer.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "../stb/stb_image.h"

#ifdef __APPLE__
constexpr s32 width = 480;
constexpr s32 height = 360;
#elif _WIN32
constexpr s32 width = 1920;
constexpr s32 height = 1080;
#endif

void InitGL() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        exit(EXIT_FAILURE);
    }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    InitGL();
    Window window{width, height};

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "glew error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    printf("%s\n", glGetString(GL_VERSION));

    Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};

    f32 verts[] = {
        -1.0, 0.0, -1.0, 1.0,
        1.0, 0.0, -1.0, 1.0,
        0.0, 1.0, -1.0, 1.0,
    };

    u32 conn[] = {
        0, 1, 2
    };

    VertexArray vertexArray{};
    vertexArray.AddVertexBuffer(new VertexBuffer(verts, sizeof(verts) / sizeof(f32),
        {{"verts", 4, 0, 0, GL_FLOAT}}));
    vertexArray.AddIndexBuffer(new IndexBuffer(conn, sizeof(conn) / sizeof(u32)));

    shader.Bind();
    shader.SetUniformMat4("projection", glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f));
    shader.SetUniformMat4("transform", glm::mat4(1.0f));
    shader.SetUniformMat4("camera", glm::lookAt(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f}));

    f64 lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window.m_glWindow)) {
        glfwPollEvents();

        f64 currentTime = glfwGetTime();
        f64 delta = (currentTime - lastTime) * 1000.0;

        lastTime = glfwGetTime();

        shader.Bind();

        vertexArray.Bind();
        glDrawElements(GL_TRIANGLES, vertexArray.IndexCount(), GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window.m_glWindow);
        //printf("%f\n", 1000.0 / delta);
    }
    return 0;
}
