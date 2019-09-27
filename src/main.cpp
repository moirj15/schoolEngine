#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
//#include <GLUT/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/transform.hpp>

#include <thread>
#include <chrono>
#include <string>


#include "window.h"
#include "shader.h"

#include "VertexBuffer.h"

#include "obj.h"
#include "keyframe.h"
#include "boundingbox.h"

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

#define ArraySize(X) (sizeof(X) / sizeof(X[0]))

Window *InitGL() {
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
    Window *window = new Window{width, height};

    glfwMakeContextCurrent(window->m_glWindow);
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "glew error: %s\n", glewGetErrorString(err));
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPolygonMode(GL_FRONT, GL_FILL);
    glClearDepth(4.0);
    glDepthFunc(GL_LESS);
    return window;
}

void InitIMGUI(Window *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplGlfw_InitForOpenGL(window->m_glWindow, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    Window *window = InitGL();
    InitIMGUI(window);

    printf("%s\n", glGetString(GL_VERSION));

#ifndef __APPLE__
    Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};
#else
    Shader shader{{"../shaders/macshader.vert", "../shaders/macshader.frag"}};
#endif

    Ray ray{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)};
    BoundingBox bb{glm::vec3(-1.0f, -1.0f, -5.0f), glm::vec3(1.0f, 1.0f, -2.0f)};
    bool intersect = bb.IntersectRay(ray);

    ObjReader objReader{"../objData/block.obj"};
    std::unique_ptr<Mesh> mesh{objReader.Parse()};

    VertexArray boxObjVA{};
    boxObjVA.AddVertexBuffer(new VertexBuffer(mesh->vertecies.data(), 
        mesh->vertecies.size() , {{"boxVerts", 3, 0, 0, GL_FLOAT}}));
    boxObjVA.AddVertexBuffer(new VertexBuffer(boxColors, ArraySize(boxColors),
        {{"boxColors", 3, 0, 1, GL_FLOAT}}));
    boxObjVA.AddIndexBuffer(new IndexBuffer(mesh->connections.data(), mesh->connections.size()));

    shader.Bind();
    shader.SetUniformMat4("projection", glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f));
    shader.SetUniformMat4("transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)));
    shader.SetUniformMat4("camera", glm::lookAt(glm::vec3{0.0f, 0.0f, 0.0f},
                                                glm::vec3{0.0f, 0.0f, -1.0f},
                                                glm::vec3{0.0f, 1.0f, 0.0f}));

    glfwSetTime(0.0);
    f64 lastTime = glfwGetTime();
    f32 t = 0.0;
    while (!glfwWindowShouldClose(window->m_glWindow)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        f64 currentTime = glfwGetTime();
        f64 delta = (currentTime - lastTime);// * 1000.0;
        t += delta;//lastTime / 60.0f;

        lastTime = glfwGetTime();

        ImGui::Begin("Controls");
        ImGui::Text("This is where the controls will go");

        ImGui::End();


        ImGui::Render();
        int display_w, display_h;
        glfwMakeContextCurrent(window->m_glWindow);
        glfwGetFramebufferSize(window->m_glWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        shader.Bind();

        boxObjVA.Bind();
        glDrawElements(GL_TRIANGLES, boxObjVA.IndexCount(), GL_UNSIGNED_INT, (void*)0);

        glfwMakeContextCurrent(window->m_glWindow);
        glfwSwapBuffers(window->m_glWindow);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
