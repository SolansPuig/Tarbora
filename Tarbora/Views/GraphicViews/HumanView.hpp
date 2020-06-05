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

#ifndef __HUMANVIEW_H_
#define __HUMANVIEW_H_

#include "Layer.hpp"
#include "GraphicView.hpp"
#include "GameLayer.hpp"

namespace Tarbora {
  class HumanView : public GraphicView
  {
  public:
    HumanView();
    ~HumanView();

    virtual void getInput() override;
    virtual void update(float delta_time) override;
    virtual void draw() override;

    virtual void pushLayer(std::shared_ptr<Layer> layer);
    virtual void removeLayer(std::shared_ptr<Layer> layer);

    std::shared_ptr<GameLayer> getGameLayer() { return game_layer_; }

  protected:
    std::shared_ptr<GameLayer> game_layer_;
    std::list<std::shared_ptr<Layer>> layers_;
  };
}

#endif // __HUMANVIEW_H_
