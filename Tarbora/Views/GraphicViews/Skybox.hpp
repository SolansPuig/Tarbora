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

#ifndef __SKYBOX_H_
#define __SKYBOX_H_

#include "SceneNode.hpp"

namespace Tarbora {
  class Scene;

  struct Sun
  {
    glm::tvec3<unsigned char> color;
    glm::vec3 position;
    float size;
    float sharpness;
  };
 
  class Skybox : public MaterialNode
  {
  public:
    Skybox(const std::string &material);

    virtual void update(Scene *scene, float);

    glm::vec3 ambient{glm::vec3(0.6f, 0.5f, 0.5f)};
    glm::vec3 diffuse{glm::vec3(0.4f, 0.4f, 0.4f)};
    glm::vec3 specular{glm::vec3(2.0f, 1.0f, 0.8f)};
    glm::vec3 direction{glm::vec3(-1.0f, -4.0f, 6.0f)};

    glm::tvec3<unsigned char> sky_color{glm::tvec3<unsigned char>(150, 222, 255)};
    glm::tvec3<unsigned char> horizon_color{glm::tvec3<unsigned char>(204, 204, 204)};
    float horizon_height{1.f};
    float exponent{20};

    Sun sun[5];
  };
}

#endif // __SKYBOX_H_
