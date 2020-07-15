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

#include "Skybox.hpp"

namespace Tarbora {
  Skybox::Skybox(const std::string &material)
    : MaterialNode("sky", "sky")
  {
    setMaterial(material);

    auto mesh = std::make_shared<MeshNode>("sky", "sky_box");
    mesh->setRenderPass(RenderPass::Sky);
    mesh->setShape("cube.mesh");
    addChild(mesh);

    auto light = std::make_shared<LightNode>("sky", "light");
    light->setShape("cube.mesh");
    light->setShader("dir_light.shader.lua");
    light->setDirection(glm::normalize(glm::vec3(0.f) - glm::vec3(-0.3f, 0.41f, -1.f)));
    light->setAmbient(ambient);
    light->setDiffuse(diffuse);
    light->setSpecular(specular);
    mesh->addChild(light);
  }

  void Skybox::drawGuiEditor()
  {
    ImGui::Text("Target: %s", name.c_str());
    ImGui::SameLine(400.f);
    ImGui::Text("Actor: %s", owner.c_str());
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Sky", ImGuiTreeNodeFlags_DefaultOpen))
    {
      ImGui::Spacing();
      glm::vec3 cs = sky_color;
      cs /= 255.f;
      float sky[3] = {cs.x, cs.y, cs.z};
      if (ImGui::ColorEdit3("Sky Color", &sky[0]))
        sky_color = glm::make_vec3(sky) * 255.f;

      ImGui::Spacing();
      glm::vec3 ch = horizon_color;
      ch /= 255.f;
      float horizon[3] = {ch.x, ch.y, ch.z};
      if (ImGui::ColorEdit3("Horizon Color", &horizon[0]))
        horizon_color = glm::make_vec3(horizon) * 255.f;

      ImGui::Spacing();
      ImGui::DragFloat("Horizon Height", &horizon_height, 0.01f, 0.f, 2.f);

      ImGui::Spacing();
      ImGui::DragFloat("Horizon Exponent", &exponent, 1.f, 0.f, 2000.f);
    }

    if (ImGui::CollapsingHeader("Sun", ImGuiTreeNodeFlags_DefaultOpen))
    {
      ImGui::Spacing();
      glm::vec3 cs = sun[0].color;
      cs /= 255.f;
      float color[3] = {cs.x, cs.y, cs.z};
      if (ImGui::ColorEdit3("Color", &color[0]))
        sun[0].color = glm::make_vec3(color) * 255.f;

      ImGui::Spacing();
      glm::vec3 ps = sun[0].position;
      float pos[3] = {ps.x, ps.y, ps.z};
      if (ImGui::DragFloat3("Position", &pos[0], 0.01f))
      {
        sun[0].position = glm::make_vec3(pos);
      }

      ImGui::Spacing();
      ImGui::DragFloat("Size", &sun[0].size, 0.1f, 0.1f, 40.f);

      ImGui::Spacing();
      ImGui::DragFloat("Sharpness", &sun[0].sharpness, 1.f, 1.f, 1000.f);
    }
  }

  void Skybox::update(Scene *, float)
  {
    //scene->getGraphicsEngine()->getRenderer()->setDirecitonalLight(
    //  ambient, diffuse, specular, direction
    //);

    auto shader = material_->getShader();

    shader->use();
    shader->set("skyColor", sky_color);
    shader->set("horizonColor", horizon_color);
    shader->set("horizonHeight", horizon_height);
    shader->set("exponent", exponent);
    shader->set("n_suns", 0);

    for (int i = 0; i < 5; i++)
    {
      shader->set("sun[" + std::to_string(i) + "].color", glm::vec3(sun[i].color));
      shader->set("sun[" + std::to_string(i) + "].pos", sun[i].position);
      shader->set("sun[" + std::to_string(i) + "].size", sun[i].size);
      shader->set("sun[" + std::to_string(i) + "].sharpness", sun[i].sharpness);
    }
  }
}
