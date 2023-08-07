#shader vertex
#version 460 core

layout(location = 0) in vec2 a_Pos;
layout(location = 1) in vec2 a_TexCoords;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoords;
    gl_Position = vec4(a_Pos, 0.0, 1.0);
}

#shader fragment
#version 460 core
out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Background;
uniform bool u_GammaCorrection;

void main()
{
    vec3 color = vec3(texture(u_Background, v_TexCoord));

    if (u_GammaCorrection)
        color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}