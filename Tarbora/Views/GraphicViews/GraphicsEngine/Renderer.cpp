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

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include <random>
#include "../../../Framework/External/tracy/TracyOpenGL.hpp"

#define KERNEL_SIZE 8

namespace Tarbora {
  Renderer::Renderer()
  {
    glfwInit();
  }

  void Renderer::init(int width, int height)
  {
    width_ = width;
    height_ = height;
    glViewport(0, 0, width_, height_);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      LOG_ERR("Renderer: Failed to initialize GLEW");
      glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    quad_mesh_ = ResourcePtr<Mesh>("meshes/plane.mesh");
    setupGeometryPass();
    setupOcclusionPass();
    setupLightingPass();
    setupScenePass();
    setupPostprocess();
  }

  void Renderer::resize(int width, int height)
  {
    width_ = width;
    height_ = height;
    glViewport(0, 0, width_, height_);

    deleteGeometryPass();
    deleteOcclusionPass();
    deleteLightingPass();
    deleteScenePass();

    setupGeometryPass();
    setupOcclusionPass();
    setupLightingPass();
    setupScenePass();
    setupPostprocess();
  }

  Renderer::~Renderer()
  {
    deleteGeometryPass();
    deleteOcclusionPass();
    deleteLightingPass();
    deleteScenePass();
    glfwTerminate();
  }

  void Renderer::setProjectionMatrix(const glm::mat4 &m)
  {
    projection_ = m;
  }

  void Renderer::setViewMatrix(const glm::mat4 &m)
  {
    view_ = m;
  }

  void Renderer::setAmbientLight(const glm::vec3 &ambient)
  {
    lighting_shader_->use();
    lighting_shader_->set("ambient_light", ambient);
  }

  void Renderer::geometryPass()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void Renderer::occlusionPass()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_buffer_);
    glClear(GL_COLOR_BUFFER_BIT);
    occlusion_shader_->use();
    occlusion_shader_->set("projection", projection_);
    occlusion_shader_->set("view", view_);
    g_position_->bind(3);
    g_normal_->bind(0);
    noise_texture_->bind(1);
    glBindVertexArray(quad_mesh_->getId());
    glDrawArrays(GL_TRIANGLES, 0, quad_mesh_->getVertices());

    glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_buffer_);
    glClear(GL_COLOR_BUFFER_BIT);
    occlusion_blur_shader_->use();
    occlusion_blur_shader_->set("projection", projection_);
    occlusion_blur_shader_->set("view", view_);
    ssao_color_->bind(0);
    glBindVertexArray(quad_mesh_->getId());
    glDrawArrays(GL_TRIANGLES, 0, quad_mesh_->getVertices());
  }

  void Renderer::lightingPass()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    lighting_shader_->use();
    lighting_shader_->set("projection", projection_);
    lighting_shader_->set("view", view_);
    g_normal_->bind(0);
    g_color_spec_->bind(1);
    ssao_blur_color_->bind(2);
    glBindVertexArray(quad_mesh_->getId());
    glDrawArrays(GL_TRIANGLES, 0, quad_mesh_->getVertices());

    glEnable(GL_BLEND);
    glCullFace(GL_FRONT);
  }

  void Renderer::scenePass()
  {
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, scene_buffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene_shader_->use();
    scene_shader_->set("projection", projection_);
    scene_shader_->set("view", view_);
    lighting_color_->bind(0);
    glBindVertexArray(quad_mesh_->getId());
    glDrawArrays(GL_TRIANGLES, 0, quad_mesh_->getVertices());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, g_buffer_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, scene_buffer_); // write to default framebuffer
    glBlitFramebuffer(
      0, 0, width_, height_, 0, 0, width_, height_, GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );
    glBindFramebuffer(GL_FRAMEBUFFER, scene_buffer_);
  }

  void Renderer::sky()
  {
    glDepthFunc(GL_LEQUAL);
  }

  void Renderer::cleanSky()
  {
    glDepthFunc(GL_LESS);
  }

  void Renderer::setFaceCulling(bool value)
  {
    if (value)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
  }

  void Renderer::setAlpha(bool value)
  {
    if (value)
      glEnable(GL_BLEND);
    else
      glDisable(GL_BLEND);
  }

  void Renderer::postprocess()
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    postprocess_shader_->use();
    postprocess_shader_->set("projection", projection_);
    postprocess_shader_->set("view", view_);
    scene_color_->bind();
    glBindVertexArray(quad_mesh_->getId());
    glDrawArrays(GL_TRIANGLES, 0, quad_mesh_->getVertices());
  }

  void Renderer::setupGeometryPass()
  {
    // Generate the g buffer
    glGenFramebuffers(1, &g_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, g_buffer_);

    // Position color buffer
    g_position_ = std::make_unique<TextureInternal>(
      width_, height_, GL_RGB16F, GL_FLOAT, GL_RGB
    );
    g_position_->configure(GL_LINEAR, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_position_->getId(), 0
    );

    // Normal color buffer
    g_normal_ = std::make_unique<TextureInternal>(
      width_, height_, GL_RGB16F, GL_FLOAT, GL_RGB
    );
    g_normal_->configure(GL_LINEAR, GL_NEAREST);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, g_normal_->getId(), 0
    );

    // Color + Specular color buffer
    g_color_spec_ = std::make_unique<TextureInternal>(width_, height_, GL_RGBA);
    g_color_spec_->configure(GL_LINEAR, GL_NEAREST);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, g_color_spec_->getId(), 0
    );

    unsigned int attachments[3] = {
      GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2
    };
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_ERR("G Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void Renderer::deleteGeometryPass()
  {
    glDeleteRenderbuffers(1, &rbo_);
    g_position_.reset(nullptr);
    g_normal_.reset(nullptr);
    g_color_spec_.reset(nullptr);
    glDeleteFramebuffers(1, &g_buffer_);
  }



  float lerp(float a, float b, float f)
  {
    return a + f * (b - a);
  }

  void Renderer::setupOcclusionPass()
  {
    // Generate the SSAO Buffer
    glGenFramebuffers(1, &ssao_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_buffer_);

    // Color buffer
    ssao_color_ = std::make_unique<TextureInternal>(
      width_, height_, GL_RED, GL_FLOAT, GL_RGB
    );
    ssao_color_->configure(GL_NEAREST, GL_NEAREST);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_color_->getId(), 0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_ERR("Occlusion Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Generate the sample kernel
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0);
    std::default_random_engine generator;
    std::vector<glm::vec3> ssaoKernel;
    for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
    {
      glm::vec3 normal(0.0, 0.0, 1.0);
      glm::vec3 sample;
      float angle = 90;
      while (angle > 85) // Vectors that are too near the plane produce depth resolution
                         // artifacts, so we don't want them
      {
        sample = glm::vec3(
          randomFloats(generator) * 2.0 - 1.0,
          randomFloats(generator) * 2.0 - 1.0,
          randomFloats(generator)
        );
        sample = glm::normalize(sample);
        // sample *= randomFloats(generator);
        float scale = float(i) / KERNEL_SIZE;

        // scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        angle = glm::degrees(glm::acos(glm::dot(normal, sample)));
      }

      ssaoKernel.push_back(sample);
    }

    // Generate the noise texture
    std::vector<glm::vec3> ssaoNoise;
    for (unsigned int i = 0; i < 16; i++)
    {
      glm::vec3 noise(
        randomFloats(generator) * 2.0 - 1.0,
        randomFloats(generator) * 2.0 - 1.0,
        0.0f
      );
      ssaoNoise.push_back(noise);
    }
    noise_texture_ = std::make_unique<TextureInternal>(
      4, 4, GL_RGB32F, GL_FLOAT, GL_RGB, &ssaoNoise[0]
    );
    noise_texture_->configure(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);

    occlusion_shader_ = ResourcePtr<Shader>("shaders/occlusion.shader.lua");
    occlusion_shader_.setInitialConfig([&, ssaoKernel](auto shader){
      LOG_DEBUG("Initializing occlusion shader %d %d", width_, height_);
      shader->use();
      shader->set("gPosition", 3);
      shader->set("gNormal", 0);
      shader->set("texNoise", 1);
      shader->set("screenSize", glm::vec2(width_, height_));
      for (unsigned int i = 0; i < KERNEL_SIZE; ++i)
        shader->set("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    });

    glGenFramebuffers(1, &ssao_blur_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, ssao_blur_buffer_);

    ssao_blur_color_ = std::make_unique<TextureInternal>(
      width_, height_, GL_RED, GL_FLOAT, GL_RGB
    );
    ssao_blur_color_->configure(GL_NEAREST, GL_NEAREST);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_blur_color_->getId(), 0
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_ERR("Occlusion blur Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    occlusion_blur_shader_ = ResourcePtr<Shader>("shaders/occlusion_blur.shader.lua");
    occlusion_blur_shader_.setInitialConfig([](auto shader){
      shader->use();
      shader->set("ssaoInput", 0);
    });
  }

  void Renderer::deleteOcclusionPass()
  {
    ssao_blur_color_.reset(nullptr);
    noise_texture_.reset(nullptr);
    ssao_color_.reset(nullptr);
    glDeleteFramebuffers(1, &ssao_buffer_);
  }

  void Renderer::setupLightingPass()
  {
    // Generate the lighting buffer
    glGenFramebuffers(1, &lighting_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer_);

    // Color buffer
    lighting_color_ = std::make_unique<TextureInternal>(width_, height_, GL_RGBA);
    lighting_color_->configure(
      GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE
    );
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lighting_color_->getId(), 0
    );
    //
    //  Render buffer
    glGenRenderbuffers(1, &rbo_light_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_light_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_light_
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_ERR("Lighting Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    lighting_shader_ = ResourcePtr<Shader>("shaders/lighting.shader.lua");
    lighting_shader_.setInitialConfig([](auto shader){
      shader->use();
      shader->set("gPosition", 3);
      shader->set("gNormal", 0);
      shader->set("gColorSpec", 1);
      shader->set("ssao", 2);
    });
  }

  void Renderer::deleteLightingPass()
  {
    lighting_color_.reset(nullptr);
    glDeleteFramebuffers(1, &lighting_buffer_);
  }

  void Renderer::setupScenePass()
  {
    // Generate the scene buffer
    glGenFramebuffers(1, &scene_buffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, scene_buffer_);

    // Color buffer
    scene_color_ = std::make_unique<TextureInternal>(width_, height_, GL_RGBA);
    scene_color_->configure(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(
      GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, scene_color_->getId(), 0
    );

    // Render buffer
    glGenRenderbuffers(1, &rbo_scene_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_scene_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width_, height_);
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_scene_
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      LOG_ERR("Scene Framebuffer not complete!");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    scene_shader_ = ResourcePtr<Shader>("shaders/scene.shader.lua");
    scene_shader_.setInitialConfig([](auto shader){
      shader->use();
      shader->set("tex", 0);
    });
  }

  void Renderer::deleteScenePass()
  {
    glDeleteRenderbuffers(1, &rbo_scene_);
    scene_color_.reset(nullptr);
    glDeleteFramebuffers(1, &scene_buffer_);
  }

  void Renderer::setupPostprocess()
  {
    postprocess_shader_ = ResourcePtr<Shader>(postprocess_shader_name_);
    postprocess_shader_.setInitialConfig([](auto shader){
      shader->use();
      shader->set("scene", 0);
    });
  }
}
