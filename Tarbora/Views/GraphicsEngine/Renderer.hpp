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
    class Renderer {
    public:
        Renderer();
        ~Renderer();

        void Init(int widht, int height);

        void SetupGeometryPass();
        void SetupOcclusionPass();
        void SetupLightingPass();

        void SetupSky();
        void CleanSky();

        void SetFaceCulling(bool value);

    private:
        void GenerateDefferredFramebuffer();
        void GenerateOcclusionFramebuffer();

        int m_Width, m_Height;

        unsigned int m_gBuffer;
        std::unique_ptr<TextureInternal> m_gPosition;
        std::unique_ptr<TextureInternal> m_gNormal;
        std::unique_ptr<TextureInternal> m_gColorSpec;

        unsigned int m_ssaoBuffer;
        std::unique_ptr<TextureInternal> m_ssaoColor;
        std::unique_ptr<TextureInternal> m_NoiseTexture;
        ResourcePtr<Shader> m_OcclusionShader;

        unsigned int m_ssaoBlurBuffer;
        std::unique_ptr<TextureInternal> m_ssaoBlurColor;
        ResourcePtr<Shader> m_OcclusionBlurShader;

        ResourcePtr<Mesh> m_QuadMesh;
        ResourcePtr<Shader> m_ScreenShader;
    };
}
