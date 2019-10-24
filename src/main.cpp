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
#include "collisionManager.h"
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

static f32 verts[] = {
    -1.0,
    0.0,
    -1.0,
    1.0,
    1.0,
    0.0,
    -1.0,
    1.0,
    0.0,
    1.0,
    -1.0,
    1.0,
};

static u32 conn[] = {0, 1, 2};

static f32 boxVerts[] = {
    // front
    -1.0,
    -1.0,
    1.0,
    1.0,
    1.0,
    -1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    1.0,
    -1.0,
    1.0,
    1.0,
    1.0,
    // back
    -1.0,
    -1.0,
    -1.0,
    1.0,
    1.0,
    -1.0,
    -1.0,
    1.0,
    1.0,
    1.0,
    -1.0,
    1.0,
    -1.0,
    1.0,
    -1.0,
    1.0,
};

static f32 boxColors[] = {
    0.0,
    0.0,
    1.0,
    0.0,
    0.0,
    0.0,
    0.0,
    1.0,
    0.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    0.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
    0.0,
    1.0,
    1.0,
    1.0,
};

static u32 boxConn[] = {
    // front
    0, 1, 2, 2, 3, 0,
    // right
    1, 5, 6, 6, 2, 1,
    // back
    7, 6, 5, 5, 4, 7,
    // left
    4, 0, 3, 3, 7, 4,
    // bottom
    4, 5, 1, 1, 0, 4,
    // top
    3, 2, 6, 6, 7, 3};

#if 0

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
  (void)io;
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

  ObjReader objReader;
  //  std::unique_ptr<Mesh>
  //  sphereMesh{objReader.Parse("../objData/sphere.obj")};
  std::unique_ptr<Mesh> mesh{objReader.Parse("../objData/sphere.obj")};
  std::unique_ptr<Mesh> planeMesh{objReader.Parse("../objData/plane.obj")};

  auto perspective =
      glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.01f, 100.0f);
  glm::vec3 pos{0.0f, 3.0f, -5.0f};
  glm::vec3 direction{0.0f, -1.0f, -.00001f};
  glm::vec3 up{0.0f, 1.0f, 0.0f};
  auto camera = glm::lookAt(pos, pos + direction, up);

  auto transform =
      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
  //  Renderer::Drawable drawable = {
  //      {}, {{"transform", transform}}, &boxObjVA, &shader};
  glfwSetTime(0.0);
  f64 lastTime = glfwGetTime();
  f32 t = 0.0;

  DebugDraw::Init();

  ECS::ComponentManager componentManager;
  PhysicsManager physicsManager{&componentManager};
  ShaterableManager shaterableManager{&componentManager};
  RendererableManager rendererManager{&componentManager};
  CollisionManager collisionManager{&componentManager};

  std::vector<ECS::EntityID> billiards;
  std::vector<ECS::Transform *> transforms;
  f32 frictionCoef = 0.01f;
  for (s32 i = 0; i < 3; i++) {
    billiards.push_back(componentManager.CreateEntity(
        (u32)ECS::Type::Renderable | (u32)ECS::Type::Mesh
        | (u32)ECS::Type::Transform | (u32)ECS::Type::Physics
        | (u32)ECS::Type::Collidable));
    auto *ecsMesh = componentManager.GetComponent<ECS::Mesh>(billiards.back());
    ecsMesh->normals = mesh->normals;
    ecsMesh->vertecies = mesh->vertecies;
    ecsMesh->connections = mesh->connections;

    auto *ecsRenderable =
        componentManager.GetComponent<ECS::Renderable>(billiards.back());

    ecsRenderable->shader = nullptr;
    ecsRenderable->vertexArray = new VertexArray;
    ecsRenderable->vertexArray->AddIndexBuffer(new IndexBuffer(
        ecsMesh->connections.data(), ecsMesh->connections.size()));
    ecsRenderable->vertexArray->AddVertexBuffer(
        new VertexBuffer(ecsMesh->vertecies.data(), ecsMesh->vertecies.size(),
            {{"testVerts", 3, 0, 0, GL_FLOAT}}));
    ecsRenderable->shaderData.push_back({"color", {1.0f, 0.0f, 0.0f}});

    auto tMat = glm::scale(glm::mat4(1.0f), {0.057f, 0.057f, 0.057f});

    ecsRenderable->shaderData.push_back({"transform", tMat});

    transforms.push_back(
        componentManager.GetComponent<ECS::Transform>(billiards.back()));
    auto *collidable =
        componentManager.GetComponent<ECS::Collidable>(billiards.back());
    collidable->radius = 0.057f;
    auto *ecsPhysics =
        componentManager.GetComponent<ECS::Physics>(billiards.back());
    ecsPhysics->mass = 0.170f;
    ecsPhysics->frictionCoef = frictionCoef;
    ecsPhysics->momentum = {0.0f, 0.0f, 0.0f};
  }
  transforms[0]->position = {0.0f, 0.0f, -5.0f};
  transforms[1]->position = {-0.060f, 0.0f, -6.5f};
  transforms[2]->position = {0.060f, 0.0f, -6.5f};
  transforms[0]->prevPosition = {0.0f, 0.0f, -5.0f};
  transforms[1]->prevPosition = {-1.6f, 0.0f, -6.1f};
  transforms[2]->prevPosition = {1.6f, 0.0f, -6.1f};

  auto cueBall = componentManager.CreateEntity(
      (u32)ECS::Type::Renderable | (u32)ECS::Type::Mesh
      | (u32)ECS::Type::Transform | (u32)ECS::Type::Physics
      | (u32)ECS::Type::Collidable);

  auto *ecsMesh = componentManager.GetComponent<ECS::Mesh>(cueBall);
  ecsMesh->normals = mesh->normals;
  ecsMesh->vertecies = mesh->vertecies;
  ecsMesh->connections = mesh->connections;

  auto *ecsRenderable = componentManager.GetComponent<ECS::Renderable>(cueBall);
  auto *collidable = componentManager.GetComponent<ECS::Collidable>(cueBall);
  collidable->radius = 0.057f;

  auto *ecsPhysics = componentManager.GetComponent<ECS::Physics>(cueBall);
  ///VELOCITY
  ///
  ///
  ///
  ///
  ///
  ecsPhysics->velocity = {0.0f, 0.0f, -2.0f};
  ecsPhysics->mass = 0.170f;
  ecsPhysics->momentum = ecsPhysics->mass * ecsPhysics->velocity;
  ecsPhysics->frictionCoef = frictionCoef;

ecsRenderable->shader = nullptr;
  ecsRenderable->vertexArray = new VertexArray;
  ecsRenderable->vertexArray->AddIndexBuffer(new IndexBuffer(
      ecsMesh->connections.data(), ecsMesh->connections.size()));
  ecsRenderable->vertexArray->AddVertexBuffer(
      new VertexBuffer(ecsMesh->vertecies.data(), ecsMesh->vertecies.size(),
          {{"testVerts", 3, 0, 0, GL_FLOAT}}));
  ecsRenderable->shaderData.push_back({"color", {1.0f, 1.0f, 1.0f}});
  auto *cueTransform = componentManager.GetComponent<ECS::Transform>(cueBall);
  cueTransform->position = {0.0f, 0.0f, -3.2f};
  cueTransform->prevPosition = {0.0f, 0.0f, -2.0f};
    auto tMat = glm::scale(glm::mat4(1.0f), {0.057f, 0.057f, 0.057f});

    ecsRenderable->shaderData.push_back({"transform", tMat});

  Renderer::Drawable planeDraw;
  planeDraw.vertexArray = new VertexArray;
  planeDraw.vertexArray->AddIndexBuffer(new IndexBuffer(
      planeMesh->connections.data(), planeMesh->connections.size()));
  planeDraw.vertexArray->AddVertexBuffer(
      new VertexBuffer(planeMesh->vertecies.data(), planeMesh->vertecies.size(),
          {{"plane", 3, 0, 0, GL_FLOAT}}));

  auto planeTransform = glm::scale(glm::mat4(1.0f), {1.298f, 0.0f, 2.483f});
  planeTransform = glm::translate(planeTransform, {0.0f, 0.0f, -2.0f});
  planeDraw.shaderData.push_back({"transform", planeTransform});
  planeDraw.shaderData.push_back({"color", {0.0f, 1.0f, 0.0f}});
  planeDraw.shader = nullptr;



  while (!glfwWindowShouldClose(window->m_glWindow)) {
    Renderer::ClearDrawQueue();
    Renderer::AddToDrawQueue(planeDraw);
    glfwPollEvents();
    f64 currentTime = glfwGetTime();
    f64 delta = (currentTime - lastTime); // * 1000.0;
    t += (f32)delta;                      // lastTime / 60.0f;
    collisionManager.Simulate((f32)lastTime, (f32)currentTime);
    physicsManager.Simulate((f32)lastTime, (f32)currentTime);
    shaterableManager.Simulate((f32)lastTime, (f32)currentTime);
    rendererManager.DrawComponents();

    // Start the Dear ImGui frame
    //    ImGui_ImplOpenGL3_NewFrame();
    //    ImGui_ImplGlfw_NewFrame();
    //    ImGui::NewFrame();

    Renderer::Clear();

    lastTime = glfwGetTime();

    //    ImGui::Begin("Controls");
    //    ImGui::Text("This is where the controls will go");

    //    ImGui::End();

    //    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(window->m_glWindow);
    glfwGetFramebufferSize(window->m_glWindow, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    //    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //    Renderer::AddToDrawQueue(drawable);
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
