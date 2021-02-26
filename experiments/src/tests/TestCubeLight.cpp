#include "TestCubeLight.h"
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    TestCubeLight::TestCubeLight()
        : m_ViewFrom {0.0f, 750.0f, 1000.0f}, 
        m_ModelCenter_1 {0.0f, 0.0f, 0.0f},
        m_ModelCenter_2 {200.0f, 0.0f, -400.0f},
        m_ModelAngle(0.0f),
        m_AmbientColor {1.0f, 1.0f, 1.0f},
        m_ModelColor_1 {0.9f, 0.8f, 0.1f, 1.0f},
        m_ModelColor_2 {0.2f, 0.2f, 0.6f, 1.0f}
        {
        
        float positions[42] = {
            // Face
            -100.0f,  100.0f,  100.0f,
             100.0f,  100.0f,  100.0f,
            -100.0f, -100.0f,  100.0f, 
             100.0f, -100.0f,  100.0f,

            // Left side
             100.0f,  100.0f, -100.0f,
             100.0f, -100.0f, -100.0f,

            // Right side
            -100.0f,  100.0f, -100.0f,
            -100.0f, -100.0f, -100.0f,

            // Top
            -100.0f,  100.0f, -100.0f,
             100.0f,  100.0f, -100.0f,

            // Bottom
            -100.0f, -100.0f, -100.0f,
             100.0f, -100.0f, -100.0f,

            // Back
            -100.0f,  100.0f, -100.0f,
             100.0f,  100.0f, -100.0f
        };

        /*
        float VertexData[14 * (3 + 4)];
        float CubeColor[4] = {0.9f, 0.8f, 0.1f, 1.0f};
        for (int i = 0; i < 14; i++)
        {
            VertexData[i*7] = positions[i*3];
            VertexData[i*7 + 1] = positions[i*3 + 1];
            VertexData[i*7 + 2] = positions[i*3 + 2];
            
            VertexData[i*7 + 3] = CubeColor[0];
            VertexData[i*7 + 4] = CubeColor[1];
            VertexData[i*7 + 5] = CubeColor[2];
            VertexData[i*7 + 6] = CubeColor[3];
        }
        */
        

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

        m_Shader = std::make_unique<Shader>("res/shaders/Colored3D.shader");
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push(3); // 3D vectors
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(float));
        
        m_Shader->Bind();
    }
    
    TestCubeLight::~TestCubeLight(){
        glDisable(GL_DEPTH_TEST);
    }

    void TestCubeLight::UpdateMVP(float ModelCenter[]){
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
            glm::vec3(ModelCenter[0], ModelCenter[1], ModelCenter[2])) * m_Model;

        m_MVP = m_Proj * m_View * m_Model;
    }

    void TestCubeLight::OnUpdate(float deltaTime){
        m_ModelAngle += 0.3 * deltaTime * glm::pi<float>();
        if (m_ModelAngle > 2 * glm::pi<float>()) {
            m_ModelAngle -= 2 * glm::pi<float>();
        }
    }

    void TestCubeLight::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniform4f(
            "u_AmbientColor", 
            m_AmbientColor[0], 
            m_AmbientColor[1],
            m_AmbientColor[2],
            1.0);

        UpdateMVP(m_ModelCenter_1);
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        m_Shader->SetUniform4f(
            "u_ModelColor", 
            m_ModelColor_1[0], 
            m_ModelColor_1[1],
            m_ModelColor_1[2],
            m_ModelColor_1[3]);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

        UpdateMVP(m_ModelCenter_2);
        m_Shader->SetUniformMat4f("u_MVP", m_MVP);
        m_Shader->SetUniform4f(
            "u_ModelColor", 
            m_ModelColor_2[0], 
            m_ModelColor_2[1],
            m_ModelColor_2[2],
            m_ModelColor_2[3]);
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestCubeLight::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::ColorEdit3("Light color", m_AmbientColor);
        ImGui::SliderFloat3("Viewing from", m_ViewFrom, -2000.0f, 2000.0f);
        ImGui::SliderFloat3("Model center 1", m_ModelCenter_1, -1000.0f, 1000.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}