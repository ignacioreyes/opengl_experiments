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

    class TestTexture : public Test
    {
    public:
        TestTexture();
        ~TestTexture();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_va;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_ib_square, m_ib_diamond;
        std::unique_ptr<Shader> m_ColorShader, m_TextureShader;
        std::unique_ptr<Texture> m_Texture;
        glm::mat4 m_Proj, m_View;
        float r;
        float increment;
        glm::vec3 m_TranslationA;
        glm::vec3 m_TranslationB;
    };
    
}