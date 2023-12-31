#include "ErrorHandler.h"

#ifdef _DEBUG
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char * file, int line)
{
    if (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}
#endif