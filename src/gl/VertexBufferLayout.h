#ifndef OPENGL_RENDERER_VERTEXBUFFERLAYOUT_H
#define OPENGL_RENDERER_VERTEXBUFFERLAYOUT_H

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    inline static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:
            return 4;
        case GL_UNSIGNED_BYTE:
            return 1;
        default:
            ASSERT(false);
            return 0;
        }
    }
}; // VertexBufferElement

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout() : m_Stride(0)
    {

    }

    template<typename T>
    void Push(unsigned int count)
    {
        ASSERT(false);
    }

    inline const std::vector<VertexBufferElement> & GetElements() const { return m_Elements; }

    inline unsigned int GetStride() const { return m_Stride; }
}; // VertexBufferLayout


// explicit template specializations for Push<>
template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
    m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
    m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}

#endif //OPENGL_RENDERER_VERTEXBUFFERLAYOUT_H
