#include "TestTexture.h"
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


namespace test {

    TestTexture::TestTexture(){        
        r = 0.0f;
        increment = 1.0f;
        m_TranslationA = glm::vec3(600, 400, 0);
        m_TranslationB = glm::vec3(800, 100, 0);

        m_Proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -100.0f, 100.0f);
        m_View = glm::translate(
            glm::mat4(1.0f), 
            glm::vec3(0, 0, 0));


        float positions[32] = {
            -289.0f, -203.0f, 0.0f, 0.0f,
             289.0f, -203.0f, 1.0f, 0.0f,
             289.0f,  203.0f, 1.0f, 1.0f,
            -289.0f,  203.0f, 0.0f, 1.0f,
            -200.0f,    0.0f, 0.0f, 0.0f,
               0.0f, -100.0f, 0.0f, 0.0f,
             200.0f,    0.0f, 0.0f, 0.0f,
               0.0f,  100.0f, 0.0f, 0.0f
        };

        unsigned int indices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        unsigned int diamond_indices[6] = {
            4, 5, 7,
            5, 6, 7
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_ColorShader = std::make_unique<Shader>("res/shaders/Color.shader");
        m_TextureShader = std::make_unique<Shader>("res/shaders/Texture.shader");
        m_va = std::make_unique<VertexArray>();

        // 2nd arg of VertexBuffer is the size in bytes of diamond_positions
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push(2); // 2D vectors
        layout.Push(2); // 2D texture map
        m_va->AddBuffer(*m_VertexBuffer, layout);

        m_ib_square = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(float));
        m_ib_diamond = std::make_unique<IndexBuffer>(diamond_indices, sizeof(diamond_indices) / sizeof(float));

        m_ColorShader->Bind();
        m_ColorShader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_TextureShader->Bind();

        m_Texture = std::make_unique<Texture>(
            "res/textures/vila_small_crop.jpg");  // 578 x 406 px
        m_Texture->Bind();
        m_TextureShader->SetUniform1i("u_Texture", 0);
    }
    
    TestTexture::~TestTexture(){}

    void TestTexture::OnUpdate(float deltaTime){
        if (r > 1.0f || r < 0.0f)
            increment = -increment;
        r += increment * deltaTime;
    }

    void TestTexture::OnRender(){
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        Renderer renderer;
        
        glm::mat4 model = glm::translate(
            glm::mat4(1.0f), m_TranslationA);

        glm::mat4 mvp = m_Proj * m_View * model;

        m_TextureShader->Bind();
        m_TextureShader->SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(*m_va, *m_ib_square, *m_TextureShader);

        m_ColorShader->Bind();
        m_ColorShader->SetUniformMat4f("u_MVP", mvp);
        m_ColorShader->SetUniform4f("u_Color", r, 0.9f, 0.2f, 0.5f);
        renderer.Draw(*m_va, *m_ib_diamond, *m_ColorShader);

        model = glm::translate(
            glm::mat4(1.0f), m_TranslationB);

        mvp = m_Proj * m_View * model;

        m_ColorShader->Bind();
        m_ColorShader->SetUniformMat4f("u_MVP", mvp);
        m_ColorShader->SetUniform4f("u_Color", r, 0.9f, 0.2f, 0.5f);
        renderer.Draw(*m_va, *m_ib_diamond, *m_ColorShader);
    }

    void TestTexture::OnImGuiRender(){
        ImGui::SliderFloat3("m_TranslationA", &m_TranslationA.x, 0.0f, 1200.0f);
        ImGui::SliderFloat3("m_TranslationB", &m_TranslationB.x, 0.0f, 1200.0f);
        ImGui::Text(
            "Application average %.3f ms/frame (%.1f FPS)",
            1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}