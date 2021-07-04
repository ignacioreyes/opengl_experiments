#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture.h"
#include "tests/Test3D.h"
#include "tests/TestFloatingHead.h"
#include "tests/TestCubeLight.h"
#include "tests/TestJelly.h"
#include "tests/TestSuzanne.h"


#define DEBUGGING 0


int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);  // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window;
    window = glfwCreateWindow(1200, 800, "Demo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // vsync

    if (glewInit() != GLEW_OK){
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    
    /*
    int* maxTextureSize = new int[1];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, maxTextureSize);
    std::cout << *maxTextureSize << std::endl;
    */

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();

    test::Test* currentTest = nullptr;
    test::TestMenu* testMenu = new test::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture>("Test Texture");
    testMenu->RegisterTest<test::Test3D>("3D test");
    testMenu->RegisterTest<test::TestFloatingHead>("Floating Head");
    testMenu->RegisterTest<test::TestCubeLight>("Rotating cubes with lighting");
    testMenu->RegisterTest<test::TestJelly>("Shaking Jelly!");
    testMenu->RegisterTest<test::TestSuzanne>("Suzanne");
    

    const char* glsl_version = "#version 460";
    ImGui_ImplOpenGL3_Init(glsl_version);

    /* Loop until the user closes the window */
    double LastFrameTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

            if (currentTest){
                double ThisFrameTime = glfwGetTime();

                double OnUpdateTime = glfwGetTime();
                currentTest->OnUpdate((float) (ThisFrameTime-LastFrameTime));
                OnUpdateTime = glfwGetTime() - OnUpdateTime;

                LastFrameTime = ThisFrameTime;

                double OnRenderTime = glfwGetTime();
                currentTest->OnRender();
                if (DEBUGGING)
                {
                    glFinish();  // For profiling purposes
                    OnRenderTime = glfwGetTime() - OnRenderTime;
                }
                
                ImGui::Begin("Menu");

                if (DEBUGGING)
                {
                    ImGui::Text(
                    "OnUpdate time %.3f ms, OnRender time %.3f ms",
                    1000.0 * OnUpdateTime, 1000.0 * OnRenderTime
                    );
                }
                
                if (currentTest != testMenu && ImGui::Button("<-")){
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
    if (currentTest != testMenu)
        delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}