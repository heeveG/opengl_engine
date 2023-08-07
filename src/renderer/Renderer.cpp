#include "Renderer.h"
#include "Shape.h"

void Renderer::Clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray & va, int count, const Shader & shader) const
{
    shader.Bind();
    va.Bind();

    glDrawArrays(GL_TRIANGLES, 0, count);
}

void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Draw(const Model & object, const Shader & shader) const
{
    object.Draw(shader);
}

void Renderer::Draw(const Shape & shape, const Shader & shader) const
{
    shape.Draw(shader);
}

