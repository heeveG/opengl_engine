#ifndef OPENGL_RENDERER_MESH_H
#define OPENGL_RENDERER_MESH_H

#include <string>
#include <vector>
#include <memory>
#include <glm.hpp>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 texCoords)
        : Position(pos), Normal(norm), TexCoords(texCoords)
    {
    }
};

class Mesh
{
private:
    // render data
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_EBO;

    // mesh data
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;

public:
    Mesh(std::vector<Vertex> && vertices, std::vector<unsigned int> && indices);

    void Draw(const Shader & shader) const;

    [[nodiscard]] inline unsigned int GetEBOCount() const
    {
        return m_EBO->GetCount();
    };
};

#endif //OPENGL_RENDERER_MESH_H
