#include "TestSuzanne.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    TestSuzanne::TestSuzanne()
        : m_ViewFrom {0.0f, 0.0f, 300.0f}, 
        m_ModelCenter {0.0f, 0.0f, -50.0f},
        m_ModelAngle(0.0f) 
        {


        m_3DModel = std::make_unique<Model>("res/models/suzanne.ply");
        UpdateMVP();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);

        m_Shader = std::make_unique<Shader>("res/shaders/Suzanne.shader");
        m_Shader->Bind();
    }
    
    TestSuzanne::~TestSuzanne(){
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
    }

    void TestSuzanne::UpdateMVP(){
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

        m_Model = glm::scale(
            glm::mat4(1.0f),
            glm::vec3(100.0f, 100.0f, 100.0f)
        );

        m_Model = glm::rotate(
            glm::mat4(1.0f),
            (float) -M_PI/2.0f,
            glm::vec3(1.0f, 0.0f, 0.0f)) * m_Model;

        m_Model = glm::rotate(
            glm::mat4(1.0f),
            m_ModelAngle,
            glm::vec3(0.0f, 1.0f, 0.0f)) * m_Model;

        m_Model = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(m_ModelCenter[0], m_ModelCenter[1], m_ModelCenter[2])) * m_Model;

        m_Normal = glm::mat3(glm::transpose(glm::inverse(m_Model)));
    }

    void TestSuzanne::OnUpdate(float deltaTime){
        glfwGetCursorPos(glfwGetCurrentContext(), &m_MouseX, &m_MouseY);
        m_ModelAngle = (float) (m_MouseX / 100.0f);
        UpdateMVP();
    }

    void TestSuzanne::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer renderer;

        m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_Model", m_Model);
        m_Shader->SetUniformMat4f("u_View", m_View);
        m_Shader->SetUniformMat4f("u_Proj", m_Proj);
        m_Shader->SetUniformMat3f("u_Normal", m_Normal);

        m_Shader->SetUniform3f("u_ModelColor", 0.8f, 0.1f, 0.3f);
        m_Shader->SetUniform3f("u_LightColor", 1.0f, 1.0f, 1.0f);
        m_Shader->SetUniform3f("u_LightPosition", 0.0f, 300.0f, 500.0f);
        m_Shader->SetUniform3f(
            "u_ViewPosition",
            m_ViewFrom[0],
            m_ViewFrom[1],
            m_ViewFrom[2]);

        m_3DModel->Draw(renderer, *m_Shader);
    }

    void TestSuzanne::OnImGuiRender(){
        // ImGui::SliderFloat("View: angle", &m_Angle, 0.0f, 6.28f);
        ImGui::Text("Mouse position: %.3f %.3f", m_MouseX, m_MouseY);
        ImGui::SliderFloat3("Viewing from", m_ViewFrom, -200.0f, 200.0f);
        ImGui::SliderFloat3("Model center", m_ModelCenter, -100.0f, 100.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}