#include "Shape.h"
#include "ShapeVertices.h"

Shape::Shape(ShapeType shapeType)
{
    m_VAO = std::make_unique<VertexArray>();

    VertexBufferLayout layout;

    if (shapeType == ShapeType::QUAD)
    {
        m_VBO = std::make_unique<VertexBuffer>(quadVertices, sizeof(quadVertices));

        layout.Push<float>(2); // position
        layout.Push<float>(2); // texture coords

        m_TriangleCount = 6;
    }
    else if (shapeType == ShapeType::CUBE)
    {
        m_VBO = std::make_unique<VertexBuffer>(cubeVertices, sizeof(cubeVertices));

        layout.Push<float>(3); // position
        layout.Push<float>(3); // normal
        layout.Push<float>(2); // texture coords

        m_TriangleCount = 36;
    }

    m_VAO->AddBuffer(*m_VBO, layout);
}

void Shape::Draw(const Shader & shader) const
{
    shader.Bind();
    m_VAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, m_TriangleCount);
}