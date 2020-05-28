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
    const glm::mat4 &transform,
    const glm::vec3 &ambient,
    const glm::vec3 &diffuse,
    const glm::vec3 &specular,
    const glm::vec3 &direction,
    const glm::vec4 &size
  )
  {
    RenderLightData data;
    data.mesh = mesh;
    data.transform = transform;
    data.diffuse = diffuse;
    data.specular = specular;
    data.direction = direction;
    data.size = size;

    ambient_light_ = ambient;
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
    for (unsigned int pass = RenderPass::Zero; pass < RenderPass::Last; ++pass)
    {
      switch (pass)
      {
        case Static:
          renderer_->geometryPass();
          renderer_->setFaceCulling(true);
          break;
        case Actor:
          break;
        case NoCulling:
          renderer_->setFaceCulling(false);
          break;
        case Sky:
          renderer_->occlusionPass();
          if (lights_.size() > 0)
            renderer_->setAmbientLight(ambient_light_/float(lights_.size()));
          else
            renderer_->setAmbientLight(ambient_light_);
          renderer_->lightingPass();
          drawLights();
          renderer_->scenePass();
          renderer_->sky();
          break;
        case Transparent:
          renderer_->cleanSky();
          break;
        default:
          break;
      }

      for (auto material : render_list_[pass])
      {
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
    ambient_light_ = {0.f, 0.f, 0.f};
    lights_.clear();
  }

  void RenderQueue::drawLights()
  {
    for (auto light : lights_)
    {
      light.mesh->bind();
      light.shader->use();
      light.shader->set("diffuse", light.diffuse);
      light.shader->set("specular", light.specular);
      light.shader->set("direction", light.direction);
      light.shader->set("size", light.size);
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
