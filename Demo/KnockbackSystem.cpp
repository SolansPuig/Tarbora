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

#include "KnockbackSystem.hpp"
#include "../Tarbora/Logic/PhysicsSystem.hpp"
#include "../Tarbora/Logic/ControllerSystem.hpp"

namespace Tarbora {
  KnockbackComponent::KnockbackComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    properties = table;
  }

  KnockbackUserComponent::KnockbackUserComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    strength = table.get<float>("strength", 100.f, true);
    range = table.get<float>("range", 50.f, true);
  }

  KnockbackSystem::KnockbackSystem(World *w) :
    System(w)
  {
    components->registerFactory("knockback", [&](auto id, auto table)
    {
      return std::make_shared<KnockbackComponent>(id, table);
    });
    components->registerFactory("knockback_user", [&](auto id, auto table)
    {
      return std::make_shared<KnockbackUserComponent>(id, table);
    });

    subscribe("picked_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<KnockbackComponent>(msg->target());
      if (comp)
      {
        components->createComponent(
          msg->owner(), "knockback_user", comp->properties
        );
        components->enableComponent<KnockbackUserComponent>(msg->owner());
      }
    });
    subscribe("dropped_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<KnockbackComponent>(msg->target());
      if (comp)
      {
        components->disableComponent<KnockbackUserComponent>(msg->owner());
        components->deleteComponent<KnockbackUserComponent>(msg->owner());
      }
    });
    subscribe("cast", MSGBIND(&KnockbackSystem::cast));
  }

  void KnockbackSystem::cast(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m->id();

    auto user = components->getComponent<KnockbackUserComponent>(id);
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
            glm::mat4 world = rb->getWorldTransform();
            glm::vec3 ldir = sight->look_direction * glm::vec3(0.f, 0.f, 1.f);
            target->applyImpulse(
              world * glm::vec4(ldir, 0.f) * user->strength,
              ray.hit_position
            );
          }
        }
      }
    }
  }
  void KnockbackSystem::update(float)
  {

  }
}
