#pragma once

#include <string>
#include <memory>
#include "../VertexArray.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../Shader.h"
#include "../Renderer.h"


class Model{
    private:
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
    public:
        Model(const std::string& filepath);
        ~Model();

        void Draw(Renderer& renderer, Shader& shader);
};
