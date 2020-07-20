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

#ifndef __DEMOLAYER_H_
#define __DEMOLAYER_H_

#include "../Tarbora/Views/GraphicViews/HumanView.hpp"

namespace Tarbora {
  class InventoryLayer;

  class DemoLayer : public GameLayer
  {
  public:
    DemoLayer(HumanView *view);
    ~DemoLayer() {}

    virtual bool getInput();

  private:
    std::shared_ptr<InventoryLayer> inventory_layer_;

    glm::vec3 movement_{0.f, 0.f, 0.f};
    glm::vec2 look_direction_{0.f, 0.f};
    bool jump_{false};
    bool pick_object_{false};
  };
}


#endif // __DEMOLAYER_H_
