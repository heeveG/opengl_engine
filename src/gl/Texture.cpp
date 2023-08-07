#include "Texture.h"

#include <utility>

Texture::Texture(const std::string & path, std::string type)
    : m_RendererID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0), m_Type(std::move(type))
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char * localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::Texture(int width, int height)
    : m_RendererID(0), m_FilePath(std::string()), m_Type(std::string()), m_Width(width), m_Height(height), m_BPP(0)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture && source) noexcept
    : m_RendererID(source.m_RendererID),
    m_FilePath(std::move(source.m_FilePath)),
    m_Width(source.m_Width), m_Height(source.m_Height), m_BPP(source.m_BPP),
    m_Type(std::move(source.m_Type))
{
    source.m_RendererID = 0;
}

Texture & Texture::operator=(Texture && other) noexcept
{
    m_RendererID = other.m_RendererID;
    m_FilePath = std::move(other.m_FilePath);
    m_Width = other.m_Width;  m_Height = other.m_Height; m_BPP = other.m_BPP;
    m_Type = std::move(other.m_Type);

    other.m_RendererID = 0;

    return *this;
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
