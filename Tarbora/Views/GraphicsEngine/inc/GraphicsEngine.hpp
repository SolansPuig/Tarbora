#pragma once
#include "Gui.hpp"
#include "Input.hpp"
#include "KeyCodes.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Material.hpp"

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

        unsigned int LoadMesh(std::vector<float> vertices);
        void DeleteMesh(unsigned int id);

        void UseShader(std::shared_ptr<Shader> shader);
        std::shared_ptr<Shader> GetShader();
        bool ShaderAvailable();

        int TakeScreenshot(const std::string &filename);

    private:
        void GenerateDefferredFramebuffer(int width, int height);
        void GenerateOcclusionFramebuffer(int width, int height);

        GraphicView *m_View;
        std::shared_ptr<Window> m_Window;
        std::unique_ptr<Gui> m_Gui;
        std::weak_ptr<Shader> m_Shader;

        unsigned int m_gBuffer;
        std::unique_ptr<TextureInternal> m_gPosition;
        std::unique_ptr<TextureInternal> m_gNormal;
        std::unique_ptr<TextureInternal> m_gColorSpec;

        unsigned int m_ssaoBuffer;
        std::unique_ptr<TextureInternal> m_ssaoColor;
        std::unique_ptr<TextureInternal> m_NoiseTexture;
        std::weak_ptr<Shader> m_OcclusionShader;

        unsigned int m_ssaoBlurBuffer;
        std::unique_ptr<TextureInternal> m_ssaoBlurColor;
        std::weak_ptr<Shader> m_OcclusionBlurShader;

        std::weak_ptr<Mesh> m_QuadMesh;
        std::weak_ptr<Shader> m_ScreenShader;
    };
}
