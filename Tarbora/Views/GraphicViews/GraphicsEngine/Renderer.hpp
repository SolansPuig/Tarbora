/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#ifndef __RENDERER_H_
#define __RENDERER_H_

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
    void resize(int width, int height);

    glm::vec2 getSize() { return glm::vec2(width_, height_); }

    void geometryPass();
    void occlusionPass();
    void lightingPass();
    void scenePass();
    void postprocess();

    void sky();
    void cleanSky();

    void setFaceCulling(bool value);
    void setAlpha(bool value);
    void setPostprocessShader(const std::string &shader)
    {
      postprocess_shader_name_ = shader;
    }

    void setProjectionMatrix(const glm::mat4 &m);
    void setViewMatrix(const glm::mat4 &m);

    void setDirecitonalLight(
      const glm::vec3 &ambient, const glm::vec3 &diffuse,
      const glm::vec3 &specular, const glm::vec3 &direction
    );
    void setAmbientLight(const glm::vec3 &ambient);

  private:
    void setupGeometryPass();
    void setupOcclusionPass();
    void setupLightingPass();
    void setupScenePass();
    void setupPostprocess();

    void deleteGeometryPass();
    void deleteOcclusionPass();
    void deleteLightingPass();
    void deleteScenePass();

    glm::mat4 projection_;
    glm::mat4 view_;

    int width_, height_;

    unsigned int g_buffer_;
    unsigned int rbo_;
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
    unsigned int rbo_light_;
    std::unique_ptr<TextureInternal> lighting_color_;
    ResourcePtr<Shader> lighting_shader_;

    unsigned int scene_buffer_;
    unsigned int rbo_scene_;
    std::unique_ptr<TextureInternal> scene_color_;
    ResourcePtr<Shader> scene_shader_;

    ResourcePtr<Mesh> quad_mesh_;
    std::string postprocess_shader_name_;
    ResourcePtr<Shader> postprocess_shader_;
  };
}

#endif // __RENDERER_H_
