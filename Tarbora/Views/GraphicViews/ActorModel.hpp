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

#ifndef __ACTORMODEL_H_
#define __ACTORMODEL_H_

#include "../../Framework/Module/Module.hpp"
#include "SceneNode.hpp"
#include "AnimationController.hpp"

namespace Tarbora {
  class ActorModel : public MaterialNode
  {
    friend class AnimationController;
    friend struct Animation;

  public:
    ActorModel(
      const ActorId &id, RenderPass render_pass,
      const std::string &model, const std::string &material
    );
    virtual ~ActorModel();

    virtual SceneNodePtr clone() const override
    {
      return std::make_shared<ActorModel>(*this);
    }

    virtual void load(const LuaTable &table, NodeMap *map) override;
    virtual void write(LuaFile *file) override;

    virtual void update(Scene *scene, float delta_time) override;

    virtual SceneNodePtr getChild(const std::string &name) override;

    void startAnimation(Animation animation, bool background=false);

    void endAnimation(const std::string &name, StopMode mode, float fade_out_timer);

    const std::string& getModel() { return model_; }
    RenderPass getRenderPass() { return render_pass_; }

  private:
    NodeMap nodes_;
    std::shared_ptr<AnimationController> animation_controller_;
    std::string model_;
  };
}

#endif // __ACTORMODEL_H_
