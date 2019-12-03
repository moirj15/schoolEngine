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
// #include "keyframe.h"
#include "camera.h"
#include "obj.h"
#include "particle.h"
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
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <string>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
//#include "../stb/stb_image.h"

#ifdef __APPLE__
constexpr s32 width = 480;
constexpr s32 height = 360;
#elif _WIN32
constexpr s32 width = 1920;
constexpr s32 height = 1080;
#else
constexpr s32 width = 720;
constexpr s32 height = 480;
#endif

Window *InitGL() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }

  //#ifdef __APPLE__
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //#endif
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
  //  glCullFace(GL_BACK);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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

  Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};

  auto perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 200.0f);

  std::unique_ptr<Camera> camera{
      new Camera({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f})};

  glfwSetTime(0.0);
  f64 lastTime = glfwGetTime();
  f32 t = 0.0;

  ECS::ComponentManager componentManager;
  PhysicsManager physicsManager{&componentManager};
  ShaterableManager shaterableManager{&componentManager};
  RendererableManager rendererManager{&componentManager};
  glEnable(GL_PROGRAM_POINT_SIZE);
  //  u32 ind[] = {0, 1, 2, 2, 1, 3};
  //  f32 poi[] = {-100.0, 0.0, -100.0, -100.0, 0.0, 100.0, 100.0, 0.0, -100.0,
  //  100.0, 0.0, 100.0};
  // VertexArray vert;
  // vert.AddIndexBuffer(new IndexBuffer(ind, 6));
  // vert.AddVertexBuffer(new VertexBuffer(poi, 12, {{"points", 3, 0, 0, GL_FLOAT}}));
  auto particles = SpawnRandomParticles(10);
  auto pva = ParticlesToVA(particles);

  while (!glfwWindowShouldClose(window->m_glWindow)) {
    Renderer::ClearDrawQueue();
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime);
    t += (f32)delta;

    //    Renderer::AddToDrawQueue(
    //        {{}, {{"color", {1.0f, 0.0f, 0.0f}}}, transformedBones.get(),
    //        nullptr});
    //    Renderer::AddToDrawQueue(
    //        {{Renderer::Command::DrawSolid}, {{"color", {1.0f, 1.0f, 1.0f}}},
    //        &vert, nullptr});

    Renderer::AddToDrawQueue({{}, {}, pva.get(), &shader});
    Renderer::Clear();

    lastTime = glfwGetTime();

    glfwMakeContextCurrent(window->m_glWindow);
    Renderer::Draw(camera->CalculateMatrix(), perspective);
    Renderer::DrawDebug(camera->CalculateMatrix(), perspective);
    Renderer::DisplayDraw(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  return 0;
}
