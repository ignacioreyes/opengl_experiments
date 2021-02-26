#pragma once

#include <vector>
#include <GL/glew.h>


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type){
        switch (type)
        {
        case GL_FLOAT:
            return 4;
        }
        return 0;
    }
};


class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
        : m_Stride(0) { };

    void Push(unsigned int count){
        m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
    inline unsigned int GetStride() const { return m_Stride; };
};
