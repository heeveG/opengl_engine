#ifndef OPENGL_RENDERER_GLWRAPPER_H
#define OPENGL_RENDERER_GLWRAPPER_H

#include <GLFW/glfw3.h>
#include <GL/glew.h>

#include "ErrorHandler.h"

class GLWrapper
{
private:
    int m_Width, m_Height;
    GLFWwindow * m_WindowHandle;

    bool m_BlurEnabled;
    bool m_PBREnabled;

public:
    GLWrapper(int width, int height);

    bool InitWindow();

    void Stop();

    inline void ToggleBlur()
    {
        m_BlurEnabled = !m_BlurEnabled;
    }

    inline void EnablePBR()
    {
        m_PBREnabled = true;
    }

    inline void EnableBlinnPhong()
    {
        m_PBREnabled = false;
    }

    [[nodiscard]] inline int GetWidth() const
    {
        return m_Width;
    };

    [[nodiscard]] inline int GetHeight() const
    {
        return m_Height;
    };

    [[nodiscard]] inline GLFWwindow * GetWindowHandle() const
    {
        return m_WindowHandle;
    };

    [[nodiscard]] inline bool IsBlurEnabled() const
    {
        return m_BlurEnabled;
    };

    [[nodiscard]] inline bool IsPBREnabled() const
    {
        return m_PBREnabled;
    };

private:
    void static KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods);
};

#endif //OPENGL_RENDERER_GLWRAPPER_H
