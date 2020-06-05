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

#ifndef __DEMOWINDOW_H_
#define __DEMOWINDOW_H_

#include "Layer.hpp"

namespace Tarbora {
  class DemoWindow : public Layer
  {
  public:
    DemoWindow(GraphicView *view, bool start_active) : Layer(view, start_active)
    {}

    void getInput() override
    {
      if (getInputManager()->getKeyDown(KEY_F6)) {
        active_ = !active_;
      }
    }

    void draw() override
    {
      ImGui::ShowDemoWindow(&active_);
    }
  };
}

#endif // __DEMOWINDOW_H_
