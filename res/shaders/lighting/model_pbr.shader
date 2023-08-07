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
    vec3 color;
};

struct SurfaceMaterial
{
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
};

mat3 CotangentFrame(vec3 N, vec3 p, vec2 uv);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

out vec4 o_FragColor;

in vec2 v_TexCoords;
in vec3 v_FragPos;
in vec3 v_Normal;

uniform vec3 u_Eye;
uniform SurfaceMaterial u_SurfaceMaterial;
uniform PointLight u_PointLight;

const float PI = 3.14159265359;

void main()
{
    // transform albedo to linear space
    vec3 albedo = pow(texture(u_SurfaceMaterial.albedoMap, v_TexCoords).rgb, vec3(2.2));
    float metallic = texture(u_SurfaceMaterial.metallicMap, v_TexCoords).r;
    float roughness = texture(u_SurfaceMaterial.roughnessMap, v_TexCoords).r;
    float ao = texture(u_SurfaceMaterial.aoMap, v_TexCoords).r;

    mat3 TBN = CotangentFrame(normalize(v_Normal), -v_FragPos, v_TexCoords);
    vec3 tangentNormal = texture(u_SurfaceMaterial.normalMap, v_TexCoords).xyz * 2.0 - 1.0;
    vec3 normal = normalize(TBN * tangentNormal); // transform from tangent to world space 

    vec3 viewDir = normalize(u_Eye - v_FragPos);

    // reflectance at normal incidence 
    // if dia-electric (non-metal) use F0 of 0.04 and the albedo color for metallic surface   
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // calculate radiance
    vec3 lightDir = normalize(u_PointLight.position - v_FragPos);
    vec3 halfVector = normalize(viewDir + lightDir);

    float distance = length(u_PointLight.position - v_FragPos);
    float attenuation = 1.0 / (distance * distance);

    vec3 radiance = u_PointLight.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, halfVector, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F = FresnelSchlick(max(dot(halfVector, viewDir), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001; // + 0.0001 to prevent zero division
    vec3 specular = numerator / denominator;

    // energy conservation
    vec3 kD = vec3(1.0) - F;
    kD *= 1.0 - metallic; // linear blend of diffuse for metallic surfaces

    // weakening factor of outward irradiance due to incident angle
    float NdotL = max(dot(normal, lightDir), 0.0);

    // calculate reflected spectral radiance
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    // add ambient light
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // Reinhard HDR tone mapping
    color = color / (color + vec3(1.0));

    o_FragColor = vec4(color, 1.0);
}

// cotangent space on-the-fly taken from http://www.thetenthplanet.de/archives/1180
// todo: implement precomputed tangets approach and measure efficiency gain
mat3 CotangentFrame(vec3 N, vec3 p, vec2 uv)
{
    vec3 dp1 = dFdx(p);
    vec3 dp2 = dFdy(p);
    vec2 duv1 = dFdx(uv);
    vec2 duv2 = dFdy(uv);

    vec3 dp2perp = cross(dp2, N);
    vec3 dp1perp = cross(N, dp1);
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
    return mat3(T * invmax, B * invmax, N);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}