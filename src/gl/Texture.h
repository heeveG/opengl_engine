#ifndef OPENGL_RENDERER_TEXTURE_H
#define OPENGL_RENDERER_TEXTURE_H

#include <string>

#include "ErrorHandler.h"
#include "stb_image.h"

class Texture
{
private:
    // members
    unsigned int m_RendererID;
    std::string m_FilePath;
    int m_Width, m_Height, m_BPP;
    std::string m_Type;

public:
    explicit Texture(const std::string & path, std::string type = "");
    Texture(int width, int height);

    Texture(const Texture &) = delete;
    Texture(&operator= (const Texture &)) = delete;

    Texture(Texture && source) noexcept;
    Texture & operator=(Texture && other) noexcept;

    ~Texture();

    void Bind(unsigned int slot = 0) const;

    void Unbind() const;

    [[nodiscard]] inline int GetWidth() const
    {
        return m_Width;
    };

    [[nodiscard]] inline int GetHeight() const
    {
        return m_Height;
    };

    [[nodiscard]] inline int GetBPP() const
    {
        return m_BPP;
    };

    [[nodiscard]] inline unsigned int GetRendererID() const
    {
        return m_RendererID;
    };

    [[nodiscard]] inline const std::string & GetType() const
    {
        return m_Type;
    };

    [[nodiscard]] inline const std::string & GetPath() const
    {
        return m_FilePath;
    };
};


#endif //OPENGL_RENDERER_TEXTURE_H
