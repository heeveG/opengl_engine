#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

out vec2 v_TexCoords;
out vec3 v_FragPos;
out vec3 v_Normal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat3 u_normMatrix;

void main()
{
    v_TexCoords = a_TexCoords;
    v_FragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    v_Normal = u_normMatrix * a_Normal;
    gl_Position = u_Proj * u_View * vec4(v_FragPos, 1.0);
}

#shader fragment
#version 460 core

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SurfaceMaterial
{
    sampler2D diffuseMap;
    sampler2D specularMap;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 o_FragColor;

in vec2 v_TexCoords;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_Eye;
uniform SurfaceMaterial u_SurfaceMaterial;
uniform PointLight u_PointLight;
uniform float u_Shininess;

void main()
{
    vec3 viewDir = normalize(u_Eye - v_FragPos);

    vec3 resColor = CalcPointLight(u_PointLight, normalize(v_Normal), v_FragPos, viewDir);

    o_FragColor = vec4(resColor, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / distance * distance;
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading using Blinn-Phong specular exponent
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
    // combing results
    vec3 ambient = light.ambient * vec3(texture(u_SurfaceMaterial.diffuseMap, v_TexCoords));
    vec3 diffuse = light.diffuse * diff * pow(texture(u_SurfaceMaterial.diffuseMap, v_TexCoords).rgb, vec3(2.2));
    vec3 specular = light.specular * spec * vec3(texture(u_SurfaceMaterial.specularMap, v_TexCoords));

    return attenuation * (ambient + diffuse + specular);
}