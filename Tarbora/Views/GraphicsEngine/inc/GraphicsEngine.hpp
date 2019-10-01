#pragma once
#include "Gui.hpp"
#include "Input.hpp"
#include "KeyCodes.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

namespace Tarbora {
    class GraphicsEngineImpl {
    public:
        GraphicsEngineImpl(GraphicView *view, std::string settingsFile);
        ~GraphicsEngineImpl();

        void InitGui();

        void BeforeDraw();
        void DrawMesh(std::shared_ptr<Mesh> mesh);
        void AfterDraw();

        void BeforeDrawSky();
        void AfterDrawSky();

        WindowPtr GetWindow();

        unsigned int CompileShader(std::string type, const char *code);
        unsigned int LinkProgram(unsigned int *ids);
        void DeleteProgram(unsigned int id);

        unsigned int LoadTexture(unsigned char *data, int width, int height, int nrComponents);
        void BindTexture(unsigned int id);
        void DeleteTexture(unsigned int id);

        unsigned int LoadMesh(std::vector<float> vertices);
        void DeleteMesh(unsigned int id);

        void UseShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> GetShader();
        bool ShaderAvailable();

        void SetCamera(const glm::vec3 &position, const glm::mat4 &view);
        void SetProjectionMatrix(const glm::mat4 &projection);

        int TakeScreenshot(const std::string &filename);

    private:
        void GenerateDefferredFramebuffer(int width, int height);
        void GenerateOcclusionFramebuffer(int width, int height);

        GraphicView *m_View;
        std::shared_ptr<Window> m_Window;
        std::unique_ptr<Gui> m_Gui;
        std::weak_ptr<Shader> m_Shader;

        unsigned int m_gBuffer;
        unsigned int m_gPosition;
        unsigned int m_gNormal;
        unsigned int m_gColorSpec;

        unsigned int m_ssaoBuffer;
        unsigned int m_ssaoColorBuffer;
        unsigned int m_NoiseTexture;
        std::weak_ptr<Shader> m_OcclusionShader;

        unsigned int m_ssaoBlurBuffer;
        unsigned int m_ssaoBlurColorBuffer;
        std::weak_ptr<Shader> m_OcclusionBlurShader;

        std::weak_ptr<Mesh> m_QuadMesh;
        std::weak_ptr<Shader> m_ScreenShader;

        glm::vec3 m_CameraPosition;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
    };
}
