#ifndef OPENGL_RENDERER_VERTEXARRAY_H
#define OPENGL_RENDERER_VERTEXARRAY_H

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();

    ~VertexArray();

    void AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout) const;

    void Bind() const;

    void Unbind() const;

    [[nodiscard]] inline unsigned int GetRendererID() const
    {
        return m_RendererID;
    };
};


#endif //OPENGL_RENDERER_VERTEXARRAY_H
