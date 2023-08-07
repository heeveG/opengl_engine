#ifndef OPENGL_RENDERER_INDEXBUFFER_H
#define OPENGL_RENDERER_INDEXBUFFER_H

#include "ErrorHandler.h"

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;

public:
    IndexBuffer(const unsigned int * data, unsigned int count);

    ~IndexBuffer();

    void Bind() const;

    void Unbind() const;

    [[nodiscard]] inline unsigned int GetCount() const { return m_Count; };
};


#endif //OPENGL_RENDERER_INDEXBUFFER_H
