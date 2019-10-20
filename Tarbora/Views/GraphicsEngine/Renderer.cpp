#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <random>

#define KERNEL_SIZE 16

namespace Tarbora {
    Renderer::Renderer()
    {
        glfwInit();
    }

    void Renderer::Init(int width, int height)
    {
        m_Width = width;
        m_Height = height;
        glViewport(0, 0, m_Width, m_Height);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            LOG_ERR("Renderer: Failed to initialize GLEW");
            glfwTerminate();
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        GenerateDefferredFramebuffer();
        GenerateOcclusionFramebuffer();
    }

    Renderer::~Renderer()
    {
        glfwTerminate();
    }

    void Renderer::SetupGeometryPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::SetupOcclusionPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        m_OcclusionShader->Use();
        m_gPosition->Bind(0);
        m_gNormal->Bind(1);
        m_NoiseTexture->Bind(2);
        glBindVertexArray(m_QuadMesh->GetId());
        glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh->GetVertices());

        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        m_OcclusionBlurShader->Use();
        m_ssaoColor->Bind();
        glBindVertexArray(m_QuadMesh->GetId());
        glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh->GetVertices());
    }

    void Renderer::SetupLightingPass()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_ScreenShader->Use();
        m_gPosition->Bind(0);
        m_gNormal->Bind(1);
        m_gColorSpec->Bind(2);
        m_ssaoBlurColor->Bind(3);
        glBindVertexArray(m_QuadMesh->GetId());
        glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh->GetVertices());

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Renderer::SetupSky()
    {
        glDepthFunc(GL_LEQUAL);
    }

    void Renderer::CleanSky()
    {
        glDepthFunc(GL_LESS);
    }

    void Renderer::SetFaceCulling(bool value)
    {
        if (value)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }

    void Renderer::GenerateDefferredFramebuffer()
    {
        // Generate the g buffer
        glGenFramebuffers(1, &m_gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

        // Position color buffer
        m_gPosition = std::unique_ptr<TextureInternal>(new TextureInternal(m_Width, m_Height, GL_RGB16F, GL_FLOAT, GL_RGB));
        m_gPosition->Configure(GL_LINEAR, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition->GetId(), 0);

        // Normal color buffer
        m_gNormal = std::unique_ptr<TextureInternal>(new TextureInternal(m_Width, m_Height, GL_RGB16F, GL_FLOAT, GL_RGB));
        m_gNormal->Configure(GL_LINEAR, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal->GetId(), 0);

        // Color + Specular color buffer
        m_gColorSpec = std::unique_ptr<TextureInternal>(new TextureInternal(m_Width, m_Height, GL_RGBA));
        m_gColorSpec->Configure(GL_LINEAR, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gColorSpec->GetId(), 0);

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Width, m_Height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("G Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_QuadMesh = ResourcePtr<Mesh>("meshes/plane.mesh");
        m_ScreenShader = ResourcePtr<Shader>("shaders/screen.shader.json");
        m_ScreenShader.SetInitialConfig([](auto shader){
            shader->Use();
            shader->Set("gPosition", 0);
            shader->Set("gNormal", 1);
            shader->Set("gColorSpec", 2);
            shader->Set("ssao", 3);
        });
    }


    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    void Renderer::GenerateOcclusionFramebuffer()
    {
        // Generate the SSAO Buffer
        glGenFramebuffers(1, &m_ssaoBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBuffer);

        // Color buffer
        m_ssaoColor = std::unique_ptr<TextureInternal>(new TextureInternal(m_Width, m_Height, GL_RED, GL_FLOAT, GL_RGB));
        m_ssaoColor->Configure(GL_NEAREST, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoColor->GetId(), 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("Occlusion Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Generate the sample kernel
        std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;
        std::vector<glm::vec3> ssaoKernel;
        for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
        {
            glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
            sample = glm::normalize(sample);
            // sample *= randomFloats(generator);
            float scale = float(i) / KERNEL_SIZE;

            // scale samples s.t. they're more aligned to center of kernel
            scale = lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;
            ssaoKernel.push_back(sample);
        }

        // Generate the noise texture
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < KERNEL_SIZE; i++)
        {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
            ssaoNoise.push_back(noise);
        }
        m_NoiseTexture = std::unique_ptr<TextureInternal>(new TextureInternal(4, 4, GL_RGB32F, GL_FLOAT, GL_RGB, &ssaoNoise[0]));
        m_NoiseTexture->Configure(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);

        m_OcclusionShader = ResourcePtr<Shader>("shaders/occlusion.shader.json");
        m_OcclusionShader.SetInitialConfig([&, ssaoKernel](auto shader){
            LOG_DEBUG("Initializing occlusion shader %d %d", m_Width, m_Height);
            shader->Use();
            shader->Set("gPosition", 0);
            shader->Set("gNormal", 1);
            shader->Set("texNoise", 2);
            shader->Set("screenSize", glm::vec2(m_Width, m_Height));
            for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
                shader->Set("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        });

        glGenFramebuffers(1, &m_ssaoBlurBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurBuffer);

        m_ssaoBlurColor = std::unique_ptr<TextureInternal>(new TextureInternal(m_Width, m_Height, GL_RED, GL_FLOAT, GL_RGB));
        m_ssaoBlurColor->Configure(GL_NEAREST, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurColor->GetId(), 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("Occlusion blur Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_OcclusionBlurShader = ResourcePtr<Shader>("shaders/occlusion_blur.shader.json");
        m_OcclusionBlurShader.SetInitialConfig([](auto shader){
            shader->Use();
            shader->Set("ssaoInput", 0);
        });
    }
}
