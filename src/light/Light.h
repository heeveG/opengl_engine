#ifndef OPENGL_RENDERER_LIGHT_H
#define OPENGL_RENDERER_LIGHT_H

struct PointLight
{
    // light posiiton
    glm::vec3 position;
    // light intensities/colors
    glm::vec3 ambient, diffuse, specular;

};

struct PointLightPBR
{
    // light posiiton
    glm::vec3 position;
    // radiant flux
    glm::vec3 color;

};

#endif // OPENGL_RENDERER_LIGHT_H
