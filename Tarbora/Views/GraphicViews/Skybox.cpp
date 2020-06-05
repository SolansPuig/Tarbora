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
#include "Scene.hpp"

namespace Tarbora {
  Skybox::Skybox(const std::string &material)
    : MaterialNode("sky", "sky", material)
  {
    auto mesh = std::make_shared<MeshNode>(
      "sky", "sky_box", RenderPass::Sky, "cube.mesh"
    );
    addChild(mesh);
  }

  void Skybox::update(Scene *scene, float)
  {
    scene->getGraphicsEngine()->getRenderer()->setDirecitonalLight(
      ambient, diffuse, specular, direction
    );

    auto shader = material_->getShader();

    shader->set("skyColor", glm::vec3(sky_color));
    shader->set("horizonColor", glm::vec3(horizon_color));
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
