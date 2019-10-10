#include "../inc/GraphicsEngine.hpp"
#include <time.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../Framework/External/stb_image_write.h"
#include "../../../Framework/ResourceManager/inc/Json.hpp"
#include <random>

#define KERNEL_SIZE 16

namespace Tarbora {
    GraphicsEngineImpl::GraphicsEngineImpl(GraphicView *view, std::string settingsFile) :
        m_View(view)
    {
        glfwInit();

        std::string windowTitle = "Tarbora Game Engine";
        int windowWidth = 1280, windowHeight = 720;

        JsonPtr settings = GET_RESOURCE(Json, settingsFile);
        if (settings)
        {
            raw_json window;
            settings->Get("window", &window, {true});
            if (!window.empty())
            {
                settings->PushErrName("window");

                settings->Get(window, "title", &windowTitle, {true});

                settings->GetArray(window, "size", 0, &windowWidth, {true});
                settings->GetArray(window, "size", 1, &windowHeight, {true});

                settings->PopErrName();
            }
        };

        m_Window = std::unique_ptr<Window>(new Window(windowTitle.c_str(), windowWidth, windowHeight, m_View));

        glViewport(0, 0, windowWidth, windowHeight);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            LOG_ERR("GraphicsEngineImpl: Failed to initialize GLEW");
            glfwTerminate();
        }

        stbi_flip_vertically_on_write(1);

        ResourceManager::RegisterLoader(LoaderPtr(new ShaderResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new TextureResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new MeshResourceLoader()));
        ResourceManager::RegisterLoader(LoaderPtr(new MaterialResourceLoader()));

        GenerateDefferredFramebuffer(windowWidth, windowHeight);
        GenerateOcclusionFramebuffer(windowWidth, windowHeight);
    }

    GraphicsEngineImpl::~GraphicsEngineImpl()
    {
        LOG_DEBUG("Destroying Graphics Engine");
        glfwTerminate();
    }

    void GraphicsEngineImpl::InitGui()
    {
        m_Gui = std::unique_ptr<Gui>(new Gui(m_View));
    }

    void GraphicsEngineImpl::BeforeDraw()
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Geometry pass
        glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        m_Gui->BeforeDraw();
    }

    void GraphicsEngineImpl::DrawMesh(std::shared_ptr<Mesh> mesh)
    {
        glBindVertexArray(mesh->GetId());
        glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertices());
    }

    void GraphicsEngineImpl::AfterDraw()
    {
        // Occlusion
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        if (!m_OcclusionShader.expired() && !m_QuadMesh.expired())
        {
            m_OcclusionShader.lock()->Use();
            m_gPosition->Bind(0);
            m_gNormal->Bind(1);
            m_NoiseTexture->Bind(2);
            glBindVertexArray(m_QuadMesh.lock()->GetId());
            glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh.lock()->GetVertices());
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurBuffer);
        glClear(GL_COLOR_BUFFER_BIT);
        if (!m_OcclusionBlurShader.expired() && !m_QuadMesh.expired())
        {
            m_OcclusionBlurShader.lock()->Use();
            m_ssaoColor->Bind();
            glBindVertexArray(m_QuadMesh.lock()->GetId());
            glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh.lock()->GetVertices());
        }

        // Lighting pass
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (!m_ScreenShader.expired() && !m_QuadMesh.expired())
        {
            m_ScreenShader.lock()->Use();
            m_gPosition->Bind(0);
            m_gNormal->Bind(1);
            m_gColorSpec->Bind(2);
            m_ssaoBlurColor->Bind(3);
            glBindVertexArray(m_QuadMesh.lock()->GetId());
            glDrawArrays(GL_TRIANGLES, 0, m_QuadMesh.lock()->GetVertices());
        }

        m_Gui->AfterDraw();
        m_Window->Update();
    }

    void GraphicsEngineImpl::BeforeDrawSky()
    {
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_CULL_FACE);
    }

    void GraphicsEngineImpl::AfterDrawSky()
    {
        glDepthFunc(GL_LESS);
    }

    WindowPtr GraphicsEngineImpl::GetWindow()
    {
        return m_Window;
    }

    unsigned int GraphicsEngineImpl::LoadMesh(std::vector<float> vertices)
    {
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(8 * sizeof(float)));

        return VAO;
    }

    void GraphicsEngineImpl::DeleteMesh(unsigned int id)
    {
        glDeleteVertexArrays(1, &id);
    }

    void GraphicsEngineImpl::UseShader(std::shared_ptr<Shader> shader)
    {
        m_Shader = shader;
        m_Shader.lock()->Use();
    }

    std::shared_ptr<Shader> GraphicsEngineImpl::GetShader()
    {
        return m_Shader.lock();
    }

    bool GraphicsEngineImpl::ShaderAvailable()
    {
        return !m_Shader.expired();
    }

    void GraphicsEngineImpl::GenerateDefferredFramebuffer(int width, int height)
    {
        // Generate the g buffer
        glGenFramebuffers(1, &m_gBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_gBuffer);

        // Position color buffer
        m_gPosition = std::unique_ptr<TextureInternal>(new TextureInternal(width, height, GL_RGB16F, GL_FLOAT, GL_RGB));
        m_gPosition->Configure(GL_LINEAR, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPosition->GetId(), 0);

        // Normal color buffer
        m_gNormal = std::unique_ptr<TextureInternal>(new TextureInternal(width, height, GL_RGB16F, GL_FLOAT, GL_RGB));
        m_gNormal->Configure(GL_LINEAR, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormal->GetId(), 0);

        // Color + Specular color buffer
        m_gColorSpec = std::unique_ptr<TextureInternal>(new TextureInternal(width, height, GL_RGBA));
        m_gColorSpec->Configure(GL_LINEAR, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gColorSpec->GetId(), 0);

        unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        glDrawBuffers(3, attachments);

        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("G Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_QuadMesh = GET_RESOURCE(Mesh, "meshes/plane.mesh");
        m_ScreenShader = GET_RESOURCE(Shader, "shaders/screen.shader.json");

        m_ScreenShader.lock()->Use();
        m_ScreenShader.lock()->Set("gPosition", 0);
        m_ScreenShader.lock()->Set("gNormal", 1);
        m_ScreenShader.lock()->Set("gColorSpec", 2);
        m_ScreenShader.lock()->Set("ssao", 3);
    }


    float lerp(float a, float b, float f)
    {
        return a + f * (b - a);
    }

    void GraphicsEngineImpl::GenerateOcclusionFramebuffer(int width, int height)
    {
        // Generate the SSAO Buffer
        glGenFramebuffers(1, &m_ssaoBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBuffer);

        // Color buffer
        m_ssaoColor = std::unique_ptr<TextureInternal>(new TextureInternal(width, height, GL_RED, GL_FLOAT, GL_RGB));
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

        m_OcclusionShader = GET_RESOURCE(Shader, "shaders/occlusion.shader.json");
        m_OcclusionShader.lock()->Use();
        m_OcclusionShader.lock()->Set("gPosition", 0);
        m_OcclusionShader.lock()->Set("gNormal", 1);
        m_OcclusionShader.lock()->Set("texNoise", 2);
        m_OcclusionShader.lock()->Set("screenSize", glm::vec2(width, height));
        for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
            m_OcclusionShader.lock()->Set("samples[" + std::to_string(i) + "]", ssaoKernel[i]);

        glGenFramebuffers(1, &m_ssaoBlurBuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_ssaoBlurBuffer);

        m_ssaoBlurColor = std::unique_ptr<TextureInternal>(new TextureInternal(width, height, GL_RED, GL_FLOAT, GL_RGB));
        m_ssaoBlurColor->Configure(GL_NEAREST, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ssaoBlurColor->GetId(), 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERR("Occlusion blur Framebuffer not complete!");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_OcclusionBlurShader = GET_RESOURCE(Shader, "shaders/occlusion_blur.shader.json");
        m_OcclusionBlurShader.lock()->Use();
        m_OcclusionBlurShader.lock()->Set("ssaoInput", 0);
    }

    int GraphicsEngineImpl::TakeScreenshot(const std::string &filename)
    {
        // Get the window width and height and reserve memory
        int width = m_Window->GetWidth();
        int height = m_Window->GetHeight();
        char *data = (char*) malloc((size_t) (width * height * 3));

        // Configure the format for storing the pixels and read them
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glReadPixels(0, 0, width, height,  GL_RGB, GL_UNSIGNED_BYTE, data);

        // Generate the timestamp for the name
        time_t t = time(NULL);
        char buffer[30];
        strftime(buffer, 30, "_%Y%m%d_%H%M%S.png", localtime(&t));

        // Store the screnshoot and free the reserved memory
        int saved = stbi_write_png((filename + buffer).c_str(), width, height, 3, data, 0);
        free(data);

        LOG_INFO("GraphicsEngineImpl: Saved screenshot %s", (filename + buffer).c_str());
        return saved;
    }
}
