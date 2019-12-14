#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "boundingbox.h"
#include "bvhParser.h"
#include "common.h"
#include "ecs/ecs.h"
#include "keyframe.h"
#include "obj.h"
#include "particle.h"
#include "renderer/VertexBuffer.h"
#include "renderer/camera.h"
#include "renderer/debugdraw.h"
#include "renderer/renderer.h"
#include "renderer/shader.h"
#include "skeleton.h"
#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
constexpr s32 width = 1920;
constexpr s32 height = 1080;
#endif

Window *InitGL() {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to init GLFW\n");
    exit(EXIT_FAILURE);
  }

  //#ifdef __APPLE__
  // glfwWindowHint(GLFW_SAMPLES, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,
  //    GL_TRUE); // To make MacOS happy; should not be needed
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //#endif
  Window *window = new Window(width, height);

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
  // InitIMGUI(window);
  printf("%s\n", glGetString(GL_VERSION));

//  Shader shader{{"../shaders/shader.vert", "../shaders/shader.frag"}};

  auto perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 200.0f);

  std::unique_ptr<Camera> camera{
      new Camera({0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f})};

  glfwSetTime(0.0);
  f64 lastTime = glfwGetTime();
  f32 t = 0.0;

  glEnable(GL_PROGRAM_POINT_SIZE);
  // auto particles = SpawnRandomParticles(100);
  //  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  glfwSwapBuffers(window->m_glWindow);
  //  do {
  //    glfwPollEvents();
  //  } while (glfwGetKey(window->m_glWindow, GLFW_KEY_G) != GLFW_PRESS);
  while (!glfwWindowShouldClose(window->m_glWindow)) {
    renderer::ClearDrawQueue();
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime);
    t += (f32)delta;

    renderer::Clear();

    lastTime = glfwGetTime();

    renderer::Draw(
        glm::lookAt(glm::vec3{0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}),
        perspective);
    renderer::DisplayDraw(window);
  }

  // ImGui_ImplOpenGL3_Shutdown();
  // ImGui_ImplGlfw_Shutdown();
  // ImGui::DestroyContext();

  return 0;
}
