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
#include "VertexBuffer.h"
#include "boundingbox.h"
#include "bvhParser.h"
#include "common.h"
#include "debugdraw.h"
#include "ecs.h"
#include "keyframe.h"
#include "obj.h"
#include "physicsManager.h"
#include "renderer.h"
#include "rendererablemanager.h"
#include "shader.h"
#include "shaterableManager.h"
#include "skeleton.h"
#include "window.h"

#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtx/normal.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
//#include "../stb/stb_image.h"

#ifdef __APPLE__
constexpr s32 width = 480;
constexpr s32 height = 360;
#else
constexpr s32 width = 1920;
constexpr s32 height = 1080;
#endif

Window *InitGL() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }

#ifdef __APPLE__
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // To make MacOS happy; should not be needed
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
  bvh::Parser parser;
  std::unique_ptr<SkeletonNode> skeleton{parser.Parse("../Example1.bvh")};
  //  auto bones = skeleton->ApplyMatricies();
  auto nodes = skeleton->ToList();
  auto transforms = skeleton->Transformations(0);
  std::vector<std::pair<glm::vec3, glm::vec3>> bones{
      nodes.size() - 1, {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}};
  std::vector<f32> points;
  std::vector<u32> indexes;
  for (size i = 0; i < bones.size(); i++) {
    indexes.push_back(i);
    indexes.push_back(i + 1);
    bones[i].first = transforms[i] * glm::vec4(bones[i].first, 1.0f);
    bones[i].second = transforms[i + 1] * glm::vec4(bones[i].second, 1.0f);
    for (size v = 0; v < 3; v++) {
      points.push_back(bones[i].first[v]);
    }
    for (size v = 0; v < 3; v++) {
      points.push_back(bones[i].second[v]);
    }
  }

  printf("%s\n", glGetString(GL_VERSION));
  VertexArray vertexArray;
  vertexArray.AddIndexBuffer(new IndexBuffer(indexes.data(), indexes.size()));
  vertexArray.AddVertexBuffer(new VertexBuffer(
      points.data(), points.size(), {{"points", 3, 0, 0, GL_FLOAT}}));

#ifndef __APPLE__
  Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};
#else
  Shader shader{{"../shaders/macshader.vert", "../shaders/macshader.frag"}};
#endif

  auto perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 100.0f);
  auto camera = glm::lookAt(glm::vec3{0.0f, 0.0f, 5.0f},
      glm::vec3{0.0f, 0.0f, -1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

  auto transform =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

  glfwSetTime(0.0);
  f64 lastTime = glfwGetTime();
  f32 t = 0.0;

  ECS::ComponentManager componentManager;
  PhysicsManager physicsManager{&componentManager};
  ShaterableManager shaterableManager{&componentManager};
  RendererableManager rendererManager{&componentManager};
  glEnable(GL_PROGRAM_POINT_SIZE);

  while (!glfwWindowShouldClose(window->m_glWindow)) {
    Renderer::ClearDrawQueue();
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime); // * 1000.0;
    t += (f32)delta;                      // lastTime / 60.0f;
    if (t > 10.0) {
      physicsManager.Simulate((f32)lastTime, (f32)currentTime);
      shaterableManager.Simulate((f32)lastTime, (f32)currentTime);
      rendererManager.DrawComponents();
    }
    //    Renderer::AddToDrawQueue(
    //        {{}, {{"transform", glm::scale(glm::mat4(1.0f), {0.1f, 0.1f,
    //        0.1f})}},
    //            &vertexArray, nullptr});
    Renderer::AddToDrawQueue({{}, {}, &vertexArray, nullptr});

    Renderer::Clear();

    lastTime = glfwGetTime();

    glfwMakeContextCurrent(window->m_glWindow);
    Renderer::Draw(camera, perspective);
    Renderer::DrawDebug(camera, perspective);
    Renderer::DisplayDraw(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
