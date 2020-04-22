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

#ifndef __WORLD_H_
#define __WORLD_H_

#include "Component.hpp"
#include "../Framework/Framework.hpp"

#include "EntitySystem.hpp"
#include "RenderSystem.hpp"
#include "AnimationSystem.hpp"
#include "ControllerSystem.hpp"
#include "PhysicsSystem.hpp"

/**
 * \file
 * \brief Class \ref Tarbora::World.
 */

namespace Tarbora {
  /**
   * \brief The module that controls the logics of the engine.
   *
   * <b>Subscribed to:</b>
   *
   * \c create_actor (\ref Messages::CreateActor). Creates a new entity of the specified
   * type and variant. If no id is provided, uses a unique number.
   * The entity will be created on the specified position and orientation, if any.
   */
  class World : public Module
  {
  public:
    World();

    /**
     * \brief Update all the systems of the \ref Entity_Component_System.
     *
     * \pram delta_time The time, in seconds, since the last update.
     * */
    virtual void update(float delta_time);

    ComponentManager* getComponentManager() { return &components_; }

  private:
    // Message subscriptions
    void createActor(const MessageSubject &, const MessageBody &body);

    ComponentManager components_;
    long next_id_{0};

    // Systems
    EntitySystem entity_system_;
    RenderSystem render_system_;
    AnimationSystem animation_system_;
    ControllerSystem controller_system_;
    PhysicsSystem physics_system_;
  };
}

#endif // __WORLD_H_
