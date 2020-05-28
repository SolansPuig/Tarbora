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

#ifndef __RENDER_QUEUE_H_
#define __RENDER_QUEUE_H_

// #define NO_INSTANCED true

#include "../../../Framework/Global.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Renderer.hpp"

namespace Tarbora {
  enum RenderPass
  {
    Zero,
    Static = Zero,
    Actor,
    NoCulling,
    Sky,
    Transparent,
    NotRendered,
    Last
  };

  struct RenderLightData
  {
    ResourcePtr<Mesh> mesh;
    ResourcePtr<Shader> shader;
    glm::mat4 transform;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    glm::vec4 size;
  };

  typedef std::vector<RenderElementData> InstanceList;
  typedef std::unordered_map<ResourcePtr<Mesh>, InstanceList, ResourcePtrHash> MeshList;
  typedef std::unordered_map<ResourcePtr<Material>, MeshList, ResourcePtrHash> MaterialList;
  typedef std::map<unsigned int, MaterialList> RenderList;

  class RenderQueue
  {
  public:
    RenderQueue(std::shared_ptr<Renderer> renderer) :
      renderer_(renderer) {}

    void drawMesh(
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
    );

    void drawLight(
      ResourcePtr<Mesh> mesh,
      const glm::mat4 &transform,
      const glm::vec3 &ambient,
      const glm::vec3 &diffuse,
      const glm::vec3 &specular,
      const glm::vec3 &direction,
      const glm::vec4 &size
    );

    void drawOutline(
      ResourcePtr<Mesh> mesh,
      const glm::mat4 &transform,
      const glm::tvec3<unsigned char> &color,
      float thickness
    );

    void draw();

    void pushMaterial(ResourcePtr<Material> material);
    ResourcePtr<Material> getActiveMaterial();
    void popMaterial();

    void setProjectionMatrix(const glm::mat4 &m);
    void setViewMatrix(const glm::mat4 &m);

  private:
    void drawLights();
   
    RenderList render_list_;
    std::stack<ResourcePtr<Material>> material_stack_;

    std::shared_ptr<Renderer> renderer_;

    glm::mat4 projection_;
    glm::mat4 view_;


    // For lighting
    glm::vec3 ambient_light_;
    std::vector<RenderLightData> lights_;
  };
}

#endif // __RENDER_QUEUE_H_
