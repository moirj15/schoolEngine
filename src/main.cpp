#define GLM_ENABLE_EXPERIMENTAL
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "aabb.h"
#include "bvhParser.h"
#include "TriangleNeighbor.h"
#include "common.h"
#include "ecs/components.h"
#include "ecs/ecs.h"
#include "obj.h"
#include "renderer/camera.h"
#include "renderer/renderer.h"
#include "skeleton.h"
#include "window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
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
  renderer::Init();
  // InitIMGUI(window);
  printf("%s\n", glGetString(GL_VERSION));

  ObjReader reader("../objData/block.obj");
  auto blockMesh = reader.Parse();
//  auto *blockAABBTree = ConstructAABBTree(blockMesh->vertecies, blockMesh->connections);
  auto blockTriangles = ConstructEdgeList(blockMesh->vertecies, blockMesh->connections);

  auto perspective = glm::perspective(90.0f, 16.0f / 9.0f, 0.01f, 200.0f);

  std::unique_ptr<Camera> camera{
      new Camera({0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f})};

  glfwSetTime(0.0);
  f64 lastTime = glfwGetTime();
  f32 t = 0.0;

  glEnable(GL_PROGRAM_POINT_SIZE);
  auto *worldSystem = new ecs::WorldSystem();
  auto testID = worldSystem->Create(ecs::TupleType::RenderableTuple);
  auto [renderable, transform, mesh] = worldSystem->GetTuple<ecs::RenderableComponent *,
      ecs::TransformComponent *, ecs::MeshComponent *>(testID);
  mesh->m_connections = blockMesh->connections;
  mesh->m_normals = blockMesh->normals;
  mesh->m_vertecies = blockMesh->vertecies;
  mesh->m_vertexSize = blockMesh->vertexSize;
  transform->m_position = {0.0f, 0.0f, -2.0f};
  transform->m_rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  renderable->m_shader = "shader";
  renderable->m_commands.push_back(renderer::Command::DrawSolid);

  while (!glfwWindowShouldClose(window->m_glWindow)) {
    renderer::ClearDrawQueue();
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime);

    worldSystem->Update(delta);

    renderer::Clear();

    lastTime = glfwGetTime();

    renderer::Draw(
        glm::lookAt(glm::vec3{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}),
        perspective);
    renderer::DisplayDraw(window);
  }

  // ImGui_ImplOpenGL3_Shutdown();
  // ImGui_ImplGlfw_Shutdown();
  // ImGui::DestroyContext();

  return 0;
}
