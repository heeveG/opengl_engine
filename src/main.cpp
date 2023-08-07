#include <iostream>
#include <string>

#include "Model.h"
#include "Renderer.h"
#include "Framebuffer.h"
#include "GLWrapper.h"
#include "Shape.h"
#include "FileSystem.h"
#include "Light.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

int main()
{
    // Initialise GL context
    int WIDTH = 800;
    int HEIGHT = 600;

    GLWrapper glWrapper(WIDTH, HEIGHT);

    if (!glWrapper.InitWindow())
        return 1;

    //
    // Rendering preparation
    //

    // load objects
    Model object(FileSystem::GetPath("res/objects/backpack/source/backpack.obj"));
    Model objectPBR(FileSystem::GetPath("res/objects/backpack_pbr/source/backpack.obj"));

    // quad geometry
    Shape quad(ShapeType::QUAD);

    // load textures
    Texture backgroundTex(FileSystem::GetPath("res/textures/checkered_background.jpg"));

    std::vector<Model::TexturePath> modelTexturePaths = {
        { "diffuseMap",  FileSystem::GetPath("res/objects/backpack/textures/diffuse.jpg") },
        { "specularMap", FileSystem::GetPath("res/objects/backpack/textures/specular.jpg")}
    };

    std::vector<Model::TexturePath> modelTexturePathsPBR = {
        { "albedoMap",    FileSystem::GetPath("res/objects/backpack_pbr/textures/1001_albedo.jpg") },
        { "normalMap",    FileSystem::GetPath("res/objects/backpack_pbr/textures/1001_normal.png") },
        { "metallicMap",  FileSystem::GetPath("res/objects/backpack_pbr/textures/1001_metallic.jpg") },
        { "roughnessMap", FileSystem::GetPath("res/objects/backpack_pbr/textures/1001_roughness.jpg") },
        { "aoMap",        FileSystem::GetPath("res/objects/backpack_pbr/textures/1001_AO.jpg") },
    };

    object.LoadTextures(modelTexturePaths, "u_SurfaceMaterial");
    objectPBR.LoadTextures(modelTexturePathsPBR, "u_SurfaceMaterial");

    //
    // Setup scene
    //

    // light setup
    glm::vec3 lightPos(0.0f, 0.5f, 1.0f);

    PointLight    pointLight{ lightPos, glm::vec3(0.2f), glm::vec3(0.9f), glm::vec3(1.0f) };
    PointLightPBR pointLightPBR{ lightPos, glm::vec3(75.0f, 75.0f, 75.0f) };

    // MVP & camera
    glm::vec3 eye = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));

    glm::mat4 view = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), static_cast<float>(glWrapper.GetWidth()) / glWrapper.GetHeight(), 0.1f, 100.0f);

    glm::mat3 normMatrixModel;

    //
    // Compile shaders 
    //

    Shader quadShader(FileSystem::GetPath("res/shaders/quad.shader"));
    quadShader.Bind();
    quadShader.SetUniform1i("u_Background", 0);

    Shader objShader(FileSystem::GetPath("res/shaders/lighting/model.shader"));
    objShader.Bind();
    objShader.SetUniformMat4f("u_Proj", proj);
    objShader.SetUniformMat4f("u_View", view);
    objShader.SetUniformVec3fv("u_Eye", eye);
    objShader.SetUniformVec3fv("u_PointLight.position", pointLight.position);
    objShader.SetUniformVec3fv("u_PointLight.ambient", pointLight.ambient);
    objShader.SetUniformVec3fv("u_PointLight.diffuse", pointLight.diffuse);
    objShader.SetUniformVec3fv("u_PointLight.specular", pointLight.specular);
    objShader.SetUniform1f("u_Shininess", 32.0f);

    Shader objShaderPBR(FileSystem::GetPath("res/shaders/lighting/model_pbr.shader"));
    objShaderPBR.Bind();
    objShaderPBR.SetUniformMat4f("u_Proj", proj);
    objShaderPBR.SetUniformMat4f("u_View", view);
    objShaderPBR.SetUniformVec3fv("u_Eye", eye);
    objShaderPBR.SetUniformVec3fv("u_PointLight.position", pointLightPBR.position);
    objShaderPBR.SetUniformVec3fv("u_PointLight.color", pointLightPBR.color);

    // blur shader
#ifdef BLUR_MASK   // use pre-generated mask (half-black/half-white) to mask out the right half of the texture
    Texture blurMaskTex(FileSystem::GetPath("res/textures/blur_mask.jpg"));
    Shader blurShader(FileSystem::GetPath("res/shaders/postprocess/blur_mask.shader"));
    blurShader.Bind();
    blurShader.SetUniform1i("u_BlurMask", 1);
#elif BLUR_SCISSOR // enable scissor test to mask out half of the texture for post-processing
    Shader blurShader(FileSystem::GetPath("res/shaders/postprocess/blur_scissor.shader"));
#else // use condition-based approach to mask out the right half of the texture by default
    Shader blurShader(FileSystem::GetPath("res/shaders/postprocess/blur_branch.shader"));
#endif
    blurShader.Bind();
    blurShader.SetUniform1i("u_ScreenTexture", 0);

    // Custom framebuffers
    Framebuffer frameBuffers[2] = { Framebuffer(glWrapper.GetWidth(), glWrapper.GetHeight()), Framebuffer(glWrapper.GetWidth(), glWrapper.GetHeight()) };
    Framebuffer::Unbind();

    // Custom renderer object
    Renderer renderer;

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    while (!glfwWindowShouldClose(glWrapper.GetWindowHandle()))
    {
        glfwPollEvents();

        { // render the scene to a texture
            frameBuffers[0].Bind();
            backgroundTex.Bind(0);

            GLCall(glDisable(GL_DEPTH_TEST));

            quadShader.Bind();
            quadShader.SetUniform1i("u_GammaCorrection", 0);

            renderer.Clear();
            renderer.Draw(quad, quadShader);

            GLCall(glEnable(GL_DEPTH_TEST));

            model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, -0.5f, 0.0f));
            normMatrixModel = glm::mat3(transpose(inverse(model)));

            if (glWrapper.IsPBREnabled())
            {   // PBR using Cook-Torrance BRDF
                objShaderPBR.Bind();
                objShaderPBR.SetUniformMat4f("u_Model", model);
                objShaderPBR.SetUniformMat3f("u_normMatrix", normMatrixModel);

                renderer.Draw(objectPBR, objShaderPBR);
            }
            else
            {   // Blinn-Phong
                objShader.Bind();
                objShader.SetUniformMat4f("u_Model", model);
                objShader.SetUniformMat3f("u_normMatrix", normMatrixModel);

                renderer.Draw(object, objShader);
            }
        }

        // blur the image by applying two 1D Gaussian blurs (vertical and horizontal)
        if (glWrapper.IsBlurEnabled())
        {
            frameBuffers[0].GetColorTexturePtr()->Bind(0);
            frameBuffers[1].Bind();

#ifdef BLUR_SCISSOR
            GLCall(glScissor(glWrapper.GetWidth() / 2, 0, glWrapper.GetWidth() / 2, glWrapper.GetHeight()));
            GLCall(glEnable(GL_SCISSOR_TEST));
#elif BLUR_MASK 
            blurMaskTex.Bind(1);
#endif

            GLCall(glDisable(GL_DEPTH_TEST));

            blurShader.Bind();
            blurShader.SetUniformVec2fv("u_Direction", glm::vec2(1.0f, 0.0f));
            blurShader.SetUniform1f("u_ResFactor", static_cast<float>(glWrapper.GetWidth()));

            renderer.Clear();
            renderer.Draw(quad, blurShader);

            frameBuffers[1].GetColorTexturePtr()->Bind(0);
            frameBuffers[0].Bind();

            blurShader.SetUniformVec2fv("u_Direction", glm::vec2(0.0f, 1.0f));
            blurShader.SetUniform1f("u_ResFactor", static_cast<float>(glWrapper.GetHeight()));

            renderer.Clear();
            renderer.Draw(quad, blurShader);

#ifdef BLUR_SCISSOR
            GLCall(glDisable(GL_SCISSOR_TEST));
#endif
        }

        {   // render the final texture to default framebuffer
            Framebuffer::Unbind();
            frameBuffers[0].GetColorTexturePtr()->Bind(0);

            GLCall(glDisable(GL_DEPTH_TEST));

            quadShader.Bind();
            quadShader.SetUniform1i("u_GammaCorrection", 1);

            renderer.Clear();
            renderer.Draw(quad, quadShader);
        }

        glfwSwapBuffers(glWrapper.GetWindowHandle());
    }

    glWrapper.Stop();
    return 0;
}