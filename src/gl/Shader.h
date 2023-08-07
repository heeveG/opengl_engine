#ifndef OPENGL_RENDERER_SHADER_H
#define OPENGL_RENDERER_SHADER_H

#include "ErrorHandler.h"

#include <string>
#include <unordered_map>
#include <glm.hpp>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    explicit Shader(const std::string & filename);

    ~Shader();

    void Bind() const;

    void Unbind() const;

    // Uniforms
    void SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3) const;
    void SetUniform1i(const std::string & name, int v0) const;
    void SetUniform1iv(const std::string & name, unsigned int count, const int v0[]) const;
    void SetUniform1f(const std::string & name, float v0) const;
    void SetUniformVec2fv(const std::string & name, const glm::vec2 & vector) const;
    void SetUniformVec3fv(const std::string & name, const glm::vec3 & vector) const;
    void SetUniformMat4f(const std::string & name, const glm::mat4 & matrix) const;
    void SetUniformMat3f(const std::string & name, const glm::mat3 & matrix) const;
private:
    int GetUniformLocation(const std::string & name) const;

    ShaderProgramSource ParseShader(const std::string & filepath);

    static unsigned int CompileShader(unsigned int type, const std::string & source);

    static unsigned int CreateShader(const std::string & vertexShader, const std::string & fragmentShader);

};


#endif //OPENGL_RENDERER_SHADER_H
