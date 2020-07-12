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

#ifndef __GUI_H_
#define __GUI_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wpedantic"
#include "External/imgui/imgui.h"
#include "External/imgui/misc/cpp/imgui_stdlib.h"
#include "External/ImGuizmo/ImGuizmo.h"
#pragma GCC diagnostic pop

#include "External/ForkAwesome.h"

namespace Tarbora {
  class GraphicsEngine;

  class Gui
  {
  public:
    Gui(GraphicsEngine *graphicsEngine);
    ~Gui();

    void beforeDraw();
    void afterDraw();

  private:
    GraphicsEngine *graphics_engine_;
  };
}

#endif // __GUI_H_
