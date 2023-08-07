#include <iostream>

#include "Model.h"
#include "stb_image.h"

void Model::Draw(const Shader & shader) const
{
    bindTextures(shader);

    for (const auto & m_Mesh : m_Meshes)
        m_Mesh.Draw(shader);
}

void Model::LoadTextures(const std::vector<TexturePath> & texturePaths, const std::string & materialUniformName)
{
    m_LoadedTextures.clear();

    for (const auto & texturePath : texturePaths)
        m_LoadedTextures.push_back(std::make_unique<Texture>(texturePath.path, texturePath.type));

    m_MaterialUniformName = materialUniformName;
}

void Model::loadModel(const std::string & path)
{
    Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    m_FilePath = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
        processMesh(scene->mMeshes[node->mMeshes[i]], scene);

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}

void Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        glm::vec2 texCoords;
        if (mesh->mTextureCoords[0])
            texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            texCoords = glm::vec2(0.0f, 0.0f);

        vertices.emplace_back(position, normal, texCoords);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    m_Meshes.emplace_back(std::move(vertices), std::move(indices));
}


void Model::bindTextures(const Shader & shader) const
{
    shader.Bind();

    std::string textureType;

    // bind textures and set uniform samplers
    for (unsigned int i = 0; i < m_LoadedTextures.size(); i++)
    {
        textureType = m_LoadedTextures[i]->GetType();
        shader.SetUniform1i(m_MaterialUniformName + "." + textureType, static_cast<int>(i));
        m_LoadedTextures[i]->Bind(i);
    }
}