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

#include "Scene.hpp"

namespace Tarbora {
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
    virtual ~Skybox() {}

    virtual SceneNodePtr clone() const
    {
      return std::make_shared<Skybox>(*this);
    }

    virtual const std::string getType() const { return "skybox"; }

    virtual void drawGuiEditor();
    virtual void draw(Scene *scene);

    glm::vec3 ambient{0.6f, 0.5f, 0.5f};
    glm::vec3 diffuse{0.4f, 0.4f, 0.4f};
    glm::vec3 specular{2.0f, 1.0f, 0.8f};
    glm::vec3 direction{-1.0f, -4.0f, 6.0f};

    glm::vec3 sky_color{150.f, 222.f, 255.f};
    glm::vec3 horizon_color{204.f, 204.f, 204.f};
    float horizon_height{1.f};
    float exponent{20};

    Sun sun[5];

  private:
    ResourcePtr<Mesh> mesh_;
  };
}

#endif // __SKYBOX_H_
