#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../Shader.h"
#include "../IndexBuffer.h"
#include "../Texture.h"
#include "../Renderer.h"
#include "../Model.h"

#include <memory>


namespace test {

    class TestSuzanne : public Test
    {
    private:
        void UpdateMVP();
    public:
        TestSuzanne();
        ~TestSuzanne();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<Model> m_3DModel;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj, m_View, m_Model;
        glm::mat3 m_Normal;
        float m_ViewFrom[3];
        float m_ModelCenter[3];
        float m_ModelAngle;
        double m_MouseX, m_MouseY;
    };
    
}