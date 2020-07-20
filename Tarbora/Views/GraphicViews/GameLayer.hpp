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

#ifndef __GAMELAYER_H_
#define __GAMELAYER_H_

#include "Scene.hpp"
#include "Layer.hpp"

namespace Tarbora {
  class GameLayer : public Layer
  {
  public:
    GameLayer(HumanView *view, bool start_active=true);
    ~GameLayer() {}

    virtual bool getInput() override;
    virtual void update(float delta_time) override;
    virtual void draw() override;

    void setTargetId(ActorId id) { target_id_ = id; }
    const ActorId& getTargetId() const { return target_id_; }

    std::shared_ptr<Scene> getScene() { return scene_; }

  protected:
    std::shared_ptr<Scene> scene_;

    ActorId target_id_;
  };
}

#endif // __GAMELAYER_H_
