#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#if 0
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/transform.hpp>

#include <thread>
#include <chrono>

#include "window.h"
#include "shader.h"

#include "VertexBuffer.h"

#include "obj.h"
#include "keyframe.h"
#include "interpolation.h"

#define STB_IMAGE_IMPLEMENTATION
//#include "../stb/stb_image.h"

#ifdef __APPLE__
constexpr s32 width = 480;
constexpr s32 height = 360;
#elif _WIN32
constexpr s32 width = 1920;
constexpr s32 height = 1080;
#endif


static f32 verts[] = {
    -1.0, 0.0, -1.0, 1.0,
    1.0, 0.0, -1.0, 1.0,
    0.0, 1.0, -1.0, 1.0,
};

static u32 conn[] = {
    0, 1, 2
};

static f32 boxVerts[] = {
    // front
    -1.0, -1.0,  1.0, 1.0,
     1.0, -1.0,  1.0, 1.0,
     1.0,  1.0,  1.0, 1.0,
    -1.0,  1.0,  1.0, 1.0,
    // back
    -1.0, -1.0, -1.0, 1.0,
     1.0, -1.0, -1.0, 1.0,
     1.0,  1.0, -1.0, 1.0,
    -1.0,  1.0, -1.0, 1.0,
};

static f32 boxColors[] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 1.0,
};

static u32 boxConn[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // right
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // left
    4, 0, 3,
    3, 7, 4,
    // bottom
    4, 5, 1,
    1, 0, 4,
    // top
    3, 2, 6,
    6, 7, 3
};

void InitGL() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to init GLFW\n");
        exit(EXIT_FAILURE);
    }

#ifdef __APPLE__
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
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
    glfwSwapInterval(1);

    printf("%s\n", glGetString(GL_VERSION));


    Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};

    VertexArray squareVertexArray{};
    squareVertexArray.AddVertexBuffer(new VertexBuffer(boxVerts, sizeof(boxVerts) / sizeof(f32),
        {{"boxVerts", 4, 0, 0, GL_FLOAT}}));
    squareVertexArray.AddVertexBuffer(new VertexBuffer(boxColors, sizeof(boxColors) / sizeof(f32),
        {{"boxColors", 3, 0, 1, GL_FLOAT}}));
    squareVertexArray.AddIndexBuffer(new IndexBuffer(boxConn, sizeof(boxConn) / sizeof(u32)));

    shader.Bind();
    shader.SetUniformMat4("projection", glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f));
    shader.SetUniformMat4("transform", glm::mat4(1.0f));
    shader.SetUniformMat4("camera", glm::lookAt(glm::vec3{0.0f, 0.0f, 10.0f},
                                                glm::vec3{0.0f, 0.0f, -1.0f},
                                                glm::vec3{0.0f, 1.0f, 0.0f}));

    KeyFrameGroup keyFrameGroup{};
    keyFrameGroup.LoadFromFile("../keyframe-input.txt");


    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glClearDepth(4.0);
    glDepthFunc(GL_LESS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window.m_glWindow);
    do {
        glfwPollEvents();
    } while (glfwGetKey(window.m_glWindow, GLFW_KEY_G) != GLFW_PRESS);

    glfwSetTime(0.0);
    f64 lastTime = glfwGetTime();
    f32 t = 0.0;
    while (!glfwWindowShouldClose(window.m_glWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f64 currentTime = glfwGetTime();
        f64 delta = (currentTime - lastTime);// * 1000.0;
        t += delta;//lastTime / 60.0f;
        //printf("t: %f\n", t);
        glfwPollEvents();
        glm::mat4 transform = keyFrameGroup.GenerateTranslationMat(t);
        shader.SetUniformMat4("transform", transform);


        lastTime = glfwGetTime();

        shader.Bind();

        squareVertexArray.Bind();
        glDrawElements(GL_TRIANGLES, squareVertexArray.IndexCount(), GL_UNSIGNED_INT, (void*)0);

        glfwSwapBuffers(window.m_glWindow);
    }
    return 0;
}
