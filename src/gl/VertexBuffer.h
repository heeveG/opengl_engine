#ifndef OPENGL_RENDERER_VERTEXBUFFER_H
#define OPENGL_RENDERER_VERTEXBUFFER_H

#include "ErrorHandler.h"

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void * data, unsigned int size);

    ~VertexBuffer();

    void Bind() const;

    void Unbind() const;
};


#endif //OPENGL_RENDERER_VERTEXBUFFER_H
