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

#ifndef __ARDUINOVIEW_H_
#define __ARDUINOVIEW_H_

#include "../../Framework/Module/Module.hpp"

namespace Tarbora {
  class ArduinoView : public Module
  {
  public:
    ArduinoView();
    ~ArduinoView();

    virtual void update(float delta_time) override;

  private:
    std::fstream arduino_;

    glm::vec3 value{0.f, 0.f, 0.f};
  };
}

#endif // __ARDUINOVIEW_H_
