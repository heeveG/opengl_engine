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

uniform sampler2D u_ScreenTexture;
uniform vec2 u_Direction;
uniform float u_ResFactor; // resolution factor (width or height of the bound buffer size)

const int M = 16;

const float weights[M + 1] = float[M + 1](
    0.04425662519949865,
    0.044035873841196206,
    0.043380781642569775,
    0.04231065439216247,
    0.040856643282313365,
    0.039060328279673276,
    0.0369716985390341,
    0.03464682117793548,
    0.03214534135442581,
    0.0295279624870386,
    0.02685404941667096,
    0.02417948052890078,
    0.02155484948872149,
    0.019024086115486723,
    0.016623532195728208,
    0.014381474814203989,
    0.012318109844189502
    );

void main()
{
    vec4 trueColor = texture(u_ScreenTexture, v_TexCoord);

    if (v_TexCoord.x < 0.5f)
    {
        FragColor = trueColor;
        return;
    }

    vec4 blurredColor = trueColor * weights[0];

    // minimize texture fetches using hardware linear sampling
    for (int i = 1; i < M; i += 2)
    {
        float w1 = weights[i];
        float w2 = weights[i + 1];

        float wSum = w1 + w2;
        float offset = w2 / wSum; // fetch a texel at weighted offset from pixel location

        blurredColor += texture(u_ScreenTexture, v_TexCoord + u_Direction / u_ResFactor * (float(i) + offset)) * wSum;
        blurredColor += texture(u_ScreenTexture, v_TexCoord - u_Direction / u_ResFactor * (float(i) + offset)) * wSum;
    }

    FragColor = blurredColor;
}
