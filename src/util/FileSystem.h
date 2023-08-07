#ifndef OPENGL_RENDERER_FILESYSTEM_H
#define OPENGL_RENDERER_FILESYSTEM_H

#include <string>
#include "root_directory.h"

class FileSystem
{
public:
    static std::string GetPath(const std::string & path)
    {
        return std::string(source_dir) + "/" + (path);
    }
};


#endif // OPENGL_RENDERER_FILESYSTEM_H