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

    class TestJelly : public Test
    {
    public:
        TestJelly();
        ~TestJelly();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_FloorVertexArray;
        std::unique_ptr<VertexBuffer> m_FloorVertexBuffer;
        std::unique_ptr<IndexBuffer> m_FloorIndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Proj, m_View, m_Model;
        float* m_VertexPositions;
        float* m_OldVertexPositions;

        void UpdatePositions(float deltaTime);
        void ResetPositions();
    };
    
}