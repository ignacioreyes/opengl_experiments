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

    class TestCubeLight : public Test
    {
    private:
        void UpdateMVP(float ModelCenter[]);
    public:
        TestCubeLight();
        ~TestCubeLight();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj, m_View, m_Model, m_MVP;
        float m_AmbientColor[3];
        float m_ModelColor_1[4];
        float m_ModelColor_2[4];
        
        float m_ViewFrom[3];
        float m_ModelCenter_1[3];
        float m_ModelCenter_2[3];
        float m_ModelAngle;
    };
    
}