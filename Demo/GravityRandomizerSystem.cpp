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

#include "GravityRandomizerSystem.hpp"
#include "../Tarbora/Logic/PhysicsSystem.hpp"
#include "../Tarbora/Logic/ControllerSystem.hpp"

namespace Tarbora {
  GravityRandomizerComponent::GravityRandomizerComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    properties = table;
  }

  GravityRandomizerUserComponent::GravityRandomizerUserComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    range = table.get<float>("range", 50.f, true);
  }

  GravityRandomizerSystem::GravityRandomizerSystem(World *w) :
    System(w)
  {
    components->registerFactory("gravity_randomizer", [&](auto id, auto table)
    {
      return std::make_shared<GravityRandomizerComponent>(id, table);
    });
    components->registerFactory("gravity_randomizer_user", [&](auto id, auto table)
    {
      return std::make_shared<GravityRandomizerUserComponent>(id, table);
    });

    subscribe("picked_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<GravityRandomizerComponent>(msg->target());
      if (comp)
      {
        components->createComponent(
          msg->owner(), "gravity_randomizer_user", comp->properties
        );
        components->enableComponent<GravityRandomizerUserComponent>(msg->owner());
      }
    });
    subscribe("dropped_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<GravityRandomizerComponent>(msg->target());
      if (comp)
      {
        components->disableComponent<GravityRandomizerUserComponent>(msg->owner());
        components->deleteComponent<GravityRandomizerUserComponent>(msg->owner());
      }
    });
    subscribe("cast", MSGBIND(&GravityRandomizerSystem::cast));
  }

  void GravityRandomizerSystem::cast(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m->id();

    auto user = components->getComponent<GravityRandomizerUserComponent>(id);
    if (user && user->enabled())
    {
      auto rb = components->getComponent<RigidbodyComponent>(id);
      auto sight = components->getComponent<SightComponent>(id);
      triggerLocal("move_event", Message::Event(id, "cast"));

      if (rb && sight && rb->enabled() && sight->enabled())
      {
        auto ray = rb->raycast(
          sight->eye_position,
          sight->look_direction * glm::vec3(0.f, 0.f, 1.f),
          user->range
        );

        if (ray.hit_id != "")
        {
          auto target = components->getComponent<RigidbodyComponent>(ray.hit_id);
          if (target && target->enabled())
          {
            target->setGravity(
              glm::vec3(
                rand() % 20 - 10,
                rand() % 20 - 10,
                rand() % 20 - 10
              )
            );
          }
        }
      }
    }
  }
  void GravityRandomizerSystem::update(float)
  {

  }
}
