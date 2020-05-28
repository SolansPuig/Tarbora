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

#include "ThrowSystem.hpp"
#include "../Tarbora/Logic/World.hpp"
#include "../Tarbora/Logic/EntitySystem.hpp"
#include "../Tarbora/Logic/ControllerSystem.hpp"
#include "../Tarbora/Logic/PhysicsSystem.hpp"

namespace Tarbora {
  ThrowComponent::ThrowComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    properties = table;
  }

  ThrowUserComponent::ThrowUserComponent(
    const ActorId &id, const LuaTable &table
  ) :
    Component(id, table)
  {
    projectile = table.get<std::string>("projectile");
    strength = table.get<float>("strength", 100.f, true);
    origin = table.get<glm::vec3>("origin", glm::vec3(0.f), true);
    max_projectiles = table.get<unsigned int>("max_projectiles", 3, true);
  }

  ThrowSystem::ThrowSystem(World *w) :
    System(w)
  {
    components->registerFactory("throw", [&](auto id, auto table)
    {
      return std::make_shared<ThrowComponent>(id, table);
    });
    components->registerFactory("throw_user", [&](auto id, auto table)
    {
      return std::make_shared<ThrowUserComponent>(id, table);
    });

    subscribe("picked_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<ThrowComponent>(msg->target());
      if (comp)
      {
        components->createComponent(
          msg->owner(), "throw_user", comp->properties
        );
        components->enableComponent<ThrowUserComponent>(msg->owner());
      }
    });
    subscribe("dropped_item", [&](auto, auto body)
    {
      Message::Item msg(body);
      auto comp = components->getComponent<ThrowComponent>(msg->target());
      if (comp)
      {
        components->disableComponent<ThrowUserComponent>(msg->owner());
        components->deleteComponent<ThrowUserComponent>(msg->owner());
      }
    });
    subscribe("cast", MSGBIND(&ThrowSystem::cast));
  }

  void ThrowSystem::cast(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m->id();

    auto user = components->getComponent<ThrowUserComponent>(id);
    if (user && user->enabled())
    {
      auto sight = components->getComponent<SightComponent>(id);
      auto transform = components->getComponent<TransformComponent>(id);
      triggerLocal("move_event", Message::Event(id, "cast"));

      if (sight && transform && sight->enabled())
      {
        glm::quat direction = transform->orientation * sight->look_direction;

        ActorId projectile = world->createActor(
          "",
          user->projectile,
          transform->position + direction * user->origin,
          transform->orientation
        );

        auto rb = components->getComponent<RigidbodyComponent>(projectile);
        if (rb && rb->enabled())
        {
          rb->applyImpulse(
            sight->look_direction * glm::vec3(0.f, 30.f, user->strength)
          );
        }
       
        user->projectiles.push(projectile);
        if (user->projectiles.size() > user->max_projectiles)
        {
          ActorId to_delete = user->projectiles.front();
          world->deleteActor(to_delete);
          user->projectiles.pop();
        }
      }
    }
  }
  void ThrowSystem::update(float)
  {

  }
}
