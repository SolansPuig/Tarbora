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

        void init(int width, int height);

        void geometryPass();
        void occlusionPass();
        void lightingPass();
        void scenePass();
        void postprocess();

        void sky();
        void cleanSky();

        void setFaceCulling(bool value);
        void setPostprocessShader(const std::string &shader) { postprocess_shader_name_ = shader; }

        void setProjectionMatrix(const glm::mat4 &m);
        void setViewMatrix(const glm::mat4 &m);

    private:
        void setupGeometryPass();
        void setupOcclusionPass();
        void setupLightingPass();
        void setupScenePass();
        void setupPostprocess();

        glm::mat4 projection_;
        glm::mat4 view_;

        int width_, height_;

        unsigned int g_buffer_;
        std::unique_ptr<TextureInternal> g_position_;
        std::unique_ptr<TextureInternal> g_normal_;
        std::unique_ptr<TextureInternal> g_color_spec_;

        unsigned int ssao_buffer_;
        std::unique_ptr<TextureInternal> ssao_color_;
        std::unique_ptr<TextureInternal> noise_texture_;
        ResourcePtr<Shader> occlusion_shader_;

        unsigned int ssao_blur_buffer_;
        std::unique_ptr<TextureInternal> ssao_blur_color_;
        ResourcePtr<Shader> occlusion_blur_shader_;

        unsigned int lighting_buffer_;
        std::unique_ptr<TextureInternal> lighting_color_;
        ResourcePtr<Shader> lighting_shader_;

        unsigned int scene_buffer_;
        std::unique_ptr<TextureInternal> scene_color_;
        ResourcePtr<Shader> scene_shader_;

        ResourcePtr<Mesh> quad_mesh_;
        std::string postprocess_shader_name_;
        ResourcePtr<Shader> postprocess_shader_;
    };
}
