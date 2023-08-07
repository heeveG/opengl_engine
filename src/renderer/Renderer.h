#ifndef OPENGL_RENDERER_RENDERER_H
#define OPENGL_RENDERER_RENDERER_H

#include "ErrorHandler.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Mesh.h"
#include "Model.h"
#include "Shape.h"

class Renderer
{
public:
    void Clear() const;
    void Draw(const VertexArray & va, int count, const Shader & shader) const;
    void Draw(const VertexArray & va, const IndexBuffer & ib, const Shader & shader) const;

    // todo: rewrite using SFINAE or c++20 concepts
    void Draw(const Model & object, const Shader & shader) const;
    void Draw(const Shape & shape, const Shader & shader) const;
};

#endif //OPENGL_RENDERER_RENDERER_H
