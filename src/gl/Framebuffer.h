#ifndef OPENGL_RENDERER_FRAMEBUFFER_H
#define OPENGL_RENDERER_FRAMEBUFFER_H

#include "ErrorHandler.h"
#include "Texture.h"

class Framebuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_DepthStencilBufferID;
    Texture * m_ColorTexture;

public:
    Framebuffer(int width, int height);

    Framebuffer(const Framebuffer &) = delete;
    Framebuffer(&operator= (const Framebuffer &)) = delete;

    Framebuffer(Framebuffer && source) = default;
    Framebuffer & operator=(Framebuffer && other) = default;

    ~Framebuffer();

    void Bind() const;

    static void Unbind();

    [[nodiscard]] inline const Texture * GetColorTexturePtr() const
    {
        return m_ColorTexture;
    }

    [[nodiscard]] inline unsigned int GetColorAttachmentID() const
    {
        return m_ColorTexture->GetRendererID();
    }

    [[nodiscard]] inline unsigned int GetDepthStencilAttachmentID() const
    {
        return m_DepthStencilBufferID;
    }
};


#endif //OPENGL_RENDERER_FRAMEBUFFER_H
