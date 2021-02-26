#pragma once

#include "Test.h"
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../Shader.h"
#include "../IndexBuffer.h"
#include "../Texture.h"
#include "../Renderer.h"

#include <memory>


namespace test {

    class TestFloatingHead : public Test
    {
    private:
        void UpdateMVP();
    public:
        TestFloatingHead();
        ~TestFloatingHead();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;
        glm::mat4 m_Proj, m_View, m_Model, m_MVP;
        float m_ViewFrom[3];
        float m_ModelCenter[3];
        float m_ModelAngle;
        double m_MouseX, m_MouseY;
    };
    
}