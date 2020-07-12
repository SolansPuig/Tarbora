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

#ifndef __COCKATIEL_H_
#define __COCKATIEL_H_

#include "../Tarbora/Framework/Module/Module.hpp"

namespace Tarbora {
  class CockatielIA : public Module
  {
  public:
    CockatielIA();
    ~CockatielIA();

    virtual void update(float delta_time) override;

  private:
    float timer_{0.f};
    bool crest_raised_{false};
    bool scratching_{false};
    bool grabbed_{false};

    glm::vec3 my_pos_;
    glm::quat my_orientation_;
    glm::vec3 player_pos_;
    glm::quat look_direction_{};
    glm::quat old_direction_{};
    glm::quat desired_direction_{};
    float look_lerp_{1.f};
  };
}

#endif // __COCKATIEL_H_
