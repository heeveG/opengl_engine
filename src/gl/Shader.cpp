#include <fstream>
#include <sstream>
#include <iostream>

#include "Shader.h"

Shader::Shader(const std::string & filename) : m_RendererID(0), m_FilePath(filename)
{
    ShaderProgramSource source = ParseShader(filename);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string & name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache.at(name);

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));

    m_UniformLocationCache.emplace(name, location);
    return location;
}

void Shader::SetUniform4f(const std::string & name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string & name, int v0) const
{
    GLCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1iv(const std::string & name, unsigned int count, const int v0[]) const
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, v0));
}

void Shader::SetUniform1f(const std::string & name, float v0) const
{
    GLCall(glUniform1f(GetUniformLocation(name), v0));
}

void Shader::SetUniformVec2fv(const std::string & name, const glm::vec2 & vector) const
{
    GLCall(glUniform2fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetUniformVec3fv(const std::string & name, const glm::vec3 & vector) const
{
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetUniformMat4f(const std::string & name, const glm::mat4 & matrix) const
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat3f(const std::string & name, const glm::mat3 & matrix) const
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

enum class ShaderType
{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
};

ShaderProgramSource Shader::ParseShader(const std::string & filepath)
{

    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    ShaderProgramSource sps = { ss[0].str(), ss[1].str() };
    return sps;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string & source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char * src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    // Error handling
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char * log = (char *)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, log));

        std::cout
            << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << std::endl;
        std::cout << log << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string & vertexShader, const std::string & fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));

    GLint program_linked;

    GLCall(glGetProgramiv(program, GL_LINK_STATUS, &program_linked));

    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall(glGetProgramInfoLog(program, 1024, &log_length, message));
        std::cout << "Failed to link program" << std::endl;
        std::cout << message << std::endl;
    }

    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
