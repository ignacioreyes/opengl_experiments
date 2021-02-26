#include "Test3D.h"
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    Test3D::Test3D()
        : m_ViewFrom {0.0f, 0.0f, 1000.0f}, 
        m_ModelCenter {0.0f, 0.0f, 0.0f},
        m_ModelAngle(0.0f) 
        {
        
        UpdateMVP();
        float positions[28] = {
            -200.0f, -200.0f, -200.0f,  0.7f,  0.3f, 0.1f, 1.0f,
             200.0f, -200.0f, -200.0f,  0.1f,  0.6f, 0.3f, 1.0f,
               0.0f,  200.0f, -200.0f,  0.1f,  0.1f, 0.5f, 1.0f,
               0.0f,    0.0f,  200.0f,  0.9f,  0.9f, 0.9f, 1.0f
        };

        unsigned int indices[12] = {
            0, 2, 1,
            0, 1, 3,
            0, 2, 3,
            3, 1, 2
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        m_Shader = std::make_unique<Shader>("res/shaders/Shader3D.shader");
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push(3); // 3D vectors
        layout.Push(4); // RGBA colors
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(float));
        
        m_Shader->Bind();
    }
    
    Test3D::~Test3D(){
        glDisable(GL_DEPTH_TEST);
    }

    void Test3D::UpdateMVP(){
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

    void Test3D::OnUpdate(float deltaTime){
        m_ModelAngle += deltaTime * glm::pi<float>();
        if (m_ModelAngle > 2 * glm::pi<float>()) {
            m_ModelAngle -= 2 * glm::pi<float>();
        }
        UpdateMVP();
    }

    void Test3D::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void Test3D::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::SliderFloat3("Viewing from", m_ViewFrom, -2000.0f, 2000.0f);
        ImGui::SliderFloat3("Model center", m_ModelCenter, -1000.0f, 1000.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}