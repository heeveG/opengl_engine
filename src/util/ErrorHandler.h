#ifndef OPENGL_RENDERER_ERRORHANDLER_H
#define OPENGL_RENDERER_ERRORHANDLER_H

#include <GL/glew.h>

#ifdef _DEBUG
#include <iostream>
#include <signal.h>

#define ASSERT(x) if (!(x)) raise(SIGABRT)
#define GLCall(x) GLClearError(); \
    x;                            \
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char * function, const char * file, int line);

#else

#define GLCall(x) x;
#define ASSERT(x)

#endif // _DEBUG

#endif //OPENGL_RENDERER_ERRORHANDLER_H
