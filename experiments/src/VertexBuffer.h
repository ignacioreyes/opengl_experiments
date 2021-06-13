#pragma once
#include <GL/glew.h>

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size, GLenum drawMode=GL_STATIC_DRAW);
    ~VertexBuffer();

    void Update(const void* data, unsigned int size);
    void Bind() const;
    void Unbind() const;
};
