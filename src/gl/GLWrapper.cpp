#include "GLWrapper.h"

#include <iostream>

GLWrapper::GLWrapper(int width, int height) : m_Width(width), m_Height(height), m_WindowHandle(nullptr), m_BlurEnabled(true), m_PBREnabled(false) {}

bool GLWrapper::InitWindow()
{
    // GLFW setup

    if (!glfwInit())
    {
        std::cout << "GLFW initialisation failed!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_WindowHandle = glfwCreateWindow(m_Width, m_Height, "Test Window", NULL, NULL);

    if (!m_WindowHandle)
    {
        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_WindowHandle);
    glfwSetWindowUserPointer(m_WindowHandle, this);
    glfwSetKeyCallback(m_WindowHandle, GLWrapper::KeyCallback);

    // GLEW setup

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed!" << std::endl;
        glfwDestroyWindow(m_WindowHandle);
        glfwTerminate();
        m_WindowHandle = nullptr;
        return false;
    }

    GLCall(glViewport(0, 0, m_Width, m_Height));

    return true;
}

void GLWrapper::Stop()
{
    glfwDestroyWindow(m_WindowHandle);
    glfwTerminate();
}

void GLWrapper::KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    GLWrapper & glWrapperInstance = *(reinterpret_cast<GLWrapper *>(glfwGetWindowUserPointer(window)));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        glWrapperInstance.ToggleBlur();

    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        glWrapperInstance.EnableBlinnPhong();

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        glWrapperInstance.EnablePBR();
}