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

        void GeometryPass();
        void OcclusionPass();
        void LightingPass();
        void ScenePass();
        void Postprocess();

        void Sky();
        void CleanSky();

        void SetFaceCulling(bool value);
        void SetPostprocessShader(const std::string &shader) { m_PostprocessShaderName = shader; }

    private:
        void SetupGeometryPass();
        void SetupOcclusionPass();
        void SetupLightingPass();
        void SetupScenePass();
        void SetupPostprocess();

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

        unsigned int m_LightingBuffer;
        std::unique_ptr<TextureInternal> m_LightingColor;
        ResourcePtr<Shader> m_LightingShader;

        unsigned int m_SceneBuffer;
        std::unique_ptr<TextureInternal> m_SceneColor;
        ResourcePtr<Shader> m_SceneShader;

        ResourcePtr<Mesh> m_QuadMesh;
        std::string m_PostprocessShaderName;
        ResourcePtr<Shader> m_PostprocessShader;
    };
}
