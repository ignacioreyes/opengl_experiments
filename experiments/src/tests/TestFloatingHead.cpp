#include "TestFloatingHead.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    TestFloatingHead::TestFloatingHead()
        : m_ViewFrom {0.0f, 300.0f, 600.0f}, 
        m_ModelCenter {0.0f, 150.0f, 200.0f},
        m_ModelAngle(0.0f) 
        {
        UpdateMVP();
        float positions[70] = {
            // Face
            -100.0f,  100.0f,  100.0f, 0.34f, 0.75f,
             100.0f,  100.0f,  100.0f, 0.65f, 0.75f,
            -100.0f, -100.0f,  100.0f, 0.34f, 0.5f,
             100.0f, -100.0f,  100.0f, 0.65f, 0.5f,

            // Left side
             100.0f,  100.0f, -100.0f, 1.0f,   0.75f,
             100.0f, -100.0f, -100.0f, 1.0f,   0.5f,

            // Right side
            -100.0f,  100.0f, -100.0f, 0.0f,   0.75f,
            -100.0f, -100.0f, -100.0f, 0.0f,   0.5f,

            // Top
            -100.0f,  100.0f, -100.0f, 0.34f, 1.0f,
             100.0f,  100.0f, -100.0f, 0.65f, 1.0f,

            // Bottom
            -100.0f, -100.0f, -100.0f, 0.34f, 0.25f,
             100.0f, -100.0f, -100.0f, 0.65f, 0.25f,

            // Back
            -100.0f,  100.0f, -100.0f, 0.34f,  0.0f,
             100.0f,  100.0f, -100.0f, 0.65f,  0.0f
        };

        unsigned int indices[36] = {
            // Face
            0, 2, 1,
            1, 2, 3,

            // Left side
            1, 3, 4,
            3, 5, 4,

            // Right side
            0, 6, 7,
            0, 7, 2,

            // Top
            0, 1, 9,
            0, 9, 8,

            // Bottom
            2, 10, 11,
            2, 11, 3,

            // Back
            11, 10, 12,
            12, 13, 11
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        m_Shader = std::make_unique<Shader>("res/shaders/FloatingHeadShader.shader");
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push(3); // 3D vectors
        layout.Push(2); // 2D texture map
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(float));
        
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("res/textures/floating_head.jpg");
        m_Texture->Bind(3);  // Slot 3
        m_Shader->SetUniform1i("u_Texture", 3);  // Slot 3
    }
    
    TestFloatingHead::~TestFloatingHead(){
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    void TestFloatingHead::UpdateMVP(){
        // m_Proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -2000.0f, 2000.0f);
        m_Proj = glm::perspective(
            glm::radians(60.0f),
            1200.0f / 800.0f,
            1.0f,
            5000.0f);

        m_View = glm::lookAt(
            glm::vec3(m_ViewFrom[0], m_ViewFrom[1], m_ViewFrom[2]),  // camera position
            glm::vec3(0.0f, 0.0f, 0.0f),  // where is looking to
            glm::vec3(0, 1, 0));

        m_Model = glm::rotate(
            glm::mat4(1.0f),
            m_ModelAngle,
            glm::vec3(0.0f, 1.0f, 0.0f));

        m_Model = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(m_ModelCenter[0], m_ModelCenter[1], m_ModelCenter[2])) * m_Model;

        m_MVP = m_Proj * m_View * m_Model;
    }

    void TestFloatingHead::OnUpdate(float deltaTime){
        glfwGetCursorPos(glfwGetCurrentContext(), &m_MouseX, &m_MouseY);

        /*
        m_ModelAngle += 0.5 * deltaTime * glm::pi<float>();
        if (m_ModelAngle > 2 * glm::pi<float>()) {
            m_ModelAngle -= 2 * glm::pi<float>();
        }
        */
    
        m_ModelAngle = (float) (m_MouseX / 100.0f);
        UpdateMVP();
    }

    void TestFloatingHead::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestFloatingHead::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::Text("Mouse position: %.3f %.3f", m_MouseX, m_MouseY);
        ImGui::SliderFloat3("Viewing from", m_ViewFrom, -2000.0f, 2000.0f);
        ImGui::SliderFloat3("Model center", m_ModelCenter, -1000.0f, 1000.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}