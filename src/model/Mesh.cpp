#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> && vertices, std::vector<unsigned int> && indices)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices))
{
    m_VAO = std::make_unique<VertexArray>();
    m_VBO = std::make_unique<VertexBuffer>(&m_Vertices[0], static_cast<unsigned int>(m_Vertices.size() * sizeof(Vertex)));
    m_EBO = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    layout.Push<float>(2);
    m_VAO->AddBuffer(*m_VBO, layout);
}

void Mesh::Draw(const Shader & shader) const
{
    m_VAO->Bind();
    m_EBO->Bind();

    glDrawElements(GL_TRIANGLES, m_EBO->GetCount(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

