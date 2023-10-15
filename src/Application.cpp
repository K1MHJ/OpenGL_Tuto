#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mach/mach.h>
#include <signal.h>

#include "Renderer.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.hpp"
#include "tests/TestTexture2D.hpp"
#include "tests/TestInstancing.hpp"
#include "tests/TestMultiObject.hpp"

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (glfwInit() == GL_FALSE)
      return -1;

  // OpenGL Version 3.2 Core Profile を選択する
  // GL 3.2 + GLSL 150
  const char* glsl_version = "#version 330";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
  if (!window)
  {
      glfwTerminate();
      return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  glfwSwapInterval(1);

  if(glewInit() != GLEW_OK){
    std::cout << "Error!" << std::endl;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;
  {
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    // Setup Dear ImGui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    
    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;
    
    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::TestInstancing>("Instancing");
    testMenu->RegisterTest<test::TestMultiObject>("Multi");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
      renderer.Clear();
      
      // Start the Dear ImGui frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      
      if(currentTest)
      {
        currentTest->OnUpdate(0.0f);
        currentTest->OnRender();
        ImGui::Begin("Test");
        if(currentTest != testMenu && ImGui::Button("<-"))
        {
          delete currentTest;
          currentTest = testMenu;
        }
        currentTest->OnImGuiRender();
        ImGui::End();
      }
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    delete currentTest;
    if(currentTest != testMenu)
      delete testMenu;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
