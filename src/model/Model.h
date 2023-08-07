#ifndef OPENGL_RENDERER_MODEL_H
#define OPENGL_RENDERER_MODEL_H

#include <vector>
#include <string>

#include "Mesh.h"
#include "Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model
{
public:

    struct TexturePath
    {
        std::string type;
        std::string path;
    };

private:

    std::vector<std::unique_ptr<Texture>> m_LoadedTextures;
    std::vector<Mesh> m_Meshes;
    std::string m_FilePath;
    std::string m_MaterialUniformName;

public:

    explicit Model(const std::string & path)
    {
        loadModel(path);
    }

    Model(const Model &) = delete;
    Model & operator=(const Model &) = delete;

    Model(Model &&) = default;
    Model & operator=(Model &&) = default;

    void LoadTextures(const std::vector<TexturePath> & texturePaths, const std::string & materialUniformName);

    void Draw(const Shader & shader) const;

private:

    void loadModel(const std::string & path);

    void processNode(aiNode * node, const aiScene * scene);

    void processMesh(aiMesh * mesh, const aiScene * scene);

    void bindTextures(const Shader & shader) const;
};


#endif //OPENGL_RENDERER_MODEL_H
