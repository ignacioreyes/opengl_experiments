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
        m_LightColor {1.0f, 1.0f, 1.0f},
        m_LightPosition {200.0f, 1000.0f, 400.0f},
        m_ModelColor_1 {0.9f, 0.8f, 0.1f},
        m_ModelColor_2 {0.2f, 0.2f, 0.6f}
        {
        
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    };

        unsigned int indices[3*6*2];

        for (int i = 0; i < 6; i++)
        {
            int starting_vertex = 4*i;
            indices[6*i + 0] = starting_vertex + 0;
            indices[6*i + 1] = starting_vertex + 1;
            indices[6*i + 2] = starting_vertex + 2;

            indices[6*i + 3] = starting_vertex + 2;
            indices[6*i + 4] = starting_vertex + 3;
            indices[6*i + 5] = starting_vertex + 0; 
        }
        

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        m_Shader = std::make_unique<Shader>("res/shaders/Colored3D.shader");
        m_VertexArray = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
        VertexBufferLayout layout;
        layout.Push(3);  // 3D positions
        layout.Push(3);  // 3D normals
        m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(float));
        
        m_Shader->Bind();
        m_Proj = glm::perspective(
            glm::radians(60.0f),
            1200.0f / 800.0f,
            1.0f,
            5000.0f);
        m_Shader->SetUniformMat4f("u_Proj", m_Proj);
    }
    
    TestCubeLight::~TestCubeLight(){
        glDisable(GL_DEPTH_TEST);
    }

    void TestCubeLight::UpdateModel(float ModelCenter[]){
        m_View = glm::lookAt(
            glm::vec3(m_ViewFrom[0], m_ViewFrom[1], m_ViewFrom[2]),  // camera position
            glm::vec3(0.0f, 0.0f, 0.0f),  // where is looking to
            glm::vec3(0, 1, 0));

        float ScalingFactor = 200.0f;
        m_Model = glm::scale(
            glm::mat4(1.0f),
            glm::vec3(ScalingFactor));

        m_Model = glm::rotate(
            glm::mat4(1.0f),
            m_ModelAngle,
            glm::vec3(0.0f, 1.0f, 0.0f)) * m_Model;

        m_Model = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(ModelCenter[0], ModelCenter[1], ModelCenter[2])) * m_Model;
    }

    void TestCubeLight::UpdateNormalMatrix(){
        m_Normal = glm::mat3(glm::transpose(glm::inverse(m_Model)));
    }

    void TestCubeLight::UpdateView(){
        m_View = glm::lookAt(
            glm::vec3(m_ViewFrom[0], m_ViewFrom[1], m_ViewFrom[2]),  // camera position
            glm::vec3(0.0f, 0.0f, 0.0f),  // where is looking to
            glm::vec3(0, 1, 0));
    }

    void TestCubeLight::OnUpdate(float deltaTime){
        m_ModelAngle += 0.3 * deltaTime * glm::pi<float>();
        if (m_ModelAngle > 2 * glm::pi<float>()) {
            m_ModelAngle -= 2 * glm::pi<float>();
        }
        UpdateView();

        // Scene uniforms
        m_Shader->Bind();
        m_Shader->SetUniform3f(
            "u_LightColor", 
            m_LightColor[0], 
            m_LightColor[1],
            m_LightColor[2]);

        m_Shader->SetUniform3f(
            "u_LightPosition", 
            m_LightPosition[0], 
            m_LightPosition[1],
            m_LightPosition[2]);

        m_Shader->SetUniformMat4f("u_View", m_View);
    }

    void TestCubeLight::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;
        m_Shader->Bind();

        // Model 1
        UpdateModel(m_ModelCenter_1);
        UpdateNormalMatrix();
        m_Shader->SetUniformMat4f("u_Model", m_Model);
        m_Shader->SetUniform3f(
            "u_ModelColor", 
            m_ModelColor_1[0], 
            m_ModelColor_1[1],
            m_ModelColor_1[2]);
        m_Shader->SetUniformMat3f("u_Normal", m_Normal);

        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

        // Model 2
        UpdateModel(m_ModelCenter_2);
        UpdateNormalMatrix();
        m_Shader->SetUniformMat4f("u_Model", m_Model);
        m_Shader->SetUniform3f(
            "u_ModelColor", 
            m_ModelColor_2[0], 
            m_ModelColor_2[1],
            m_ModelColor_2[2]);
        renderer.Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
    }

    void TestCubeLight::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::ColorEdit3("Light color", m_LightColor);
        ImGui::SliderFloat3("Light position", m_LightPosition, -1000.f, 1000.0f);

        ImGui::SliderFloat3("Viewing from", m_ViewFrom, -2000.0f, 2000.0f);
        ImGui::SliderFloat3("Model center 1", m_ModelCenter_1, -1000.0f, 1000.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}