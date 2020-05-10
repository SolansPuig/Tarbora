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

#ifndef __ANIMATIONSYSTEM_H_
#define __ANIMATIONSYSTEM_H_

#include "Component.hpp"

namespace Tarbora {
  class AnimationComponent : public Component {
    friend class AnimationSystem;
  public:
    AnimationComponent(const ActorId &id, const LuaTable &table);
    virtual ~AnimationComponent() {}

    static ComponentId getStaticType() { return "animation"; }
    virtual ComponentId getType() const override { return getStaticType(); }

    std::string animation_controller_path;
    std::set<std::string> playing_animations;
  };

  class AnimationSystem : public System {
    friend class World;
  public:
    AnimationSystem(World *w);

  private:
    // Message subscriptions
    void event(const MessageSubject &, const MessageBody &body);
    // onEnable callbacks
    bool enableAnimation(std::shared_ptr<Component> comp);

    // Internal functions
    void parseEvent(
      AnimationComponent *comp,
      std::string animation,
      const LuaTable &event
    );

    virtual void update(float delta_time);
  };
}

#endif // __ANIMATIONSYSTEM_H_
