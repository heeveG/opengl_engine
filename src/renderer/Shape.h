#ifndef OPENGL_RENDERER_SHAPE_H
#define OPENGL_RENDERER_SHAPE_H

#include <memory>

#include "VertexArray.h"
#include "Shader.h"

enum class ShapeType
{
    QUAD,
    CUBE
};

class Shape
{
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    int m_TriangleCount;

public:
    Shape(ShapeType shapeType);
    void Draw(const Shader & shader) const;
};


#endif //OPENGL_RENDERER_SHAPE_H
