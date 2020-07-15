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

#include "RenderQueue.hpp"

namespace Tarbora {
  void RenderQueue::drawMesh(
    RenderPass render_pass,
    ResourcePtr<Mesh> mesh,
    const glm::mat4 &transform,
    const glm::tvec2<unsigned short> &uv,
    const glm::vec3 &mesh_size,
    const glm::vec3 &texture_size,
    const glm::tvec3<unsigned char> &primary,
    const glm::tvec3<unsigned char> &secondary,
    const glm::tvec3<unsigned char> &detail,
    const glm::tvec3<unsigned char> &detail2
  )
  {
    ResourcePtr<Material> material = material_stack_.top();

    RenderElementData data;
    data.transform = transform;
    data.uv_map = uv;
    data.mesh_size = mesh_size;
    data.texture_size = texture_size;
    data.color_primary = primary;
    data.color_secondary = secondary;
    data.color_detail = detail;
    data.color_detail2 = detail2;

    render_list_[render_pass][material][mesh].emplace_back(data);
  }

  void RenderQueue::drawLight(
    ResourcePtr<Mesh> mesh,
    ResourcePtr<Shader> shader,
    const glm::mat4 &transform,
    const glm::vec3 &ambient,
    const glm::vec3 &diffuse,
    const glm::vec3 &specular,
    const glm::vec3 &direction,
    float linear_attenuation,
    float quadratic_attenuation
  )
  {
    RenderLightData data;
    data.mesh = mesh;
    data.shader = shader;
    data.transform = transform;
    data.ambient = ambient;
    data.diffuse = diffuse;
    data.specular = specular;
    data.direction = direction;
    data.linear = linear_attenuation;
    data.quadratic = quadratic_attenuation;

    lights_.emplace_back(data);
  }

  void RenderQueue::drawOutline(
    ResourcePtr<Mesh> mesh,
    const glm::mat4 &transform,
    const glm::tvec3<unsigned char> &color,
    float thickness
  )
  {
    RenderElementData data;
    data.transform = transform;
    data.mesh_size.x = thickness; // To avoid creating a new struct or adding mostly
                                  // useless data to RenderElementData
    data.color_primary = color;
    render_list_[Transparent][ResourcePtr<Material>("materials/outline.mat.lua")][mesh]
      .emplace_back(data);
  }

  void RenderQueue::draw()
  {
    for (unsigned int pass = RenderPass::Begin; pass < RenderPass::End; ++pass)
    {
      switch (pass)
      {
        case Static:
          renderer_->setAlpha(false);
          renderer_->geometryPass();
          renderer_->setFaceCulling(true);
          break;
        case Actor:
          break;
        case NoCulling:
          renderer_->setFaceCulling(false);
          break;
        case Sky:
          renderer_->setFaceCulling(true);
          renderer_->occlusionPass();
          renderer_->lightingPass();
          drawLights();
          renderer_->scenePass();
          renderer_->sky();
          break;
        case Transparent:
          renderer_->setFaceCulling(false);
          renderer_->cleanSky();
          renderer_->setAlpha(true);
          break;
        default:
          break;
      }

      for (auto material : render_list_[pass])
      {
        material.first->setScreenSize(renderer_->getSize());
        material.first->bind(projection_, view_);
        for (auto mesh : material.second)
        {
          mesh.first->bind();
          for (auto instance : mesh.second)
          {
            mesh.first->addInstance(instance);
          }
          mesh.first->drawInstanced();
        }
      }
    }
    render_list_.clear();
    lights_.clear();
  }

  void RenderQueue::drawLights()
  {
    for (auto light : lights_)
    {
      light.mesh->bind();
      light.shader->use();
      light.shader->set("ambient", light.ambient);
      light.shader->set("diffuse", light.diffuse);
      light.shader->set("specular", light.specular);
      light.shader->set("direction", light.direction);
      light.shader->set("linear", light.linear);
      light.shader->set("quadratic", light.quadratic);
      light.shader->set("transform", light.transform);
      light.shader->set("view", view_);
      light.shader->set("screen", glm::vec2(renderer_->getSize()));
      light.shader->set("projection", projection_);
      light.mesh->draw();
    }
  }

  void RenderQueue::pushMaterial(ResourcePtr<Material> material)
  {
    material_stack_.push(material);
  }

  ResourcePtr<Material> RenderQueue::getActiveMaterial()
  {
    return material_stack_.top();
  }

  void RenderQueue::popMaterial()
  {
    material_stack_.pop();
  }

  void RenderQueue::setProjectionMatrix(const glm::mat4 &m)
  {
    renderer_->setProjectionMatrix(m);
    projection_ = m;
  }

  void RenderQueue::setViewMatrix(const glm::mat4 &m)
  {
    renderer_->setViewMatrix(m);
    view_ = m;
  }
}
