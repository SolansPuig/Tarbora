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

#include "EntitySystem.hpp"

namespace Tarbora {
  TypeComponent::TypeComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    for (auto type : table)
    {
      add(type.second.getAs<std::string>());
    }

    enable();
  }

  void TypeComponent::add(const std::string &type)
  {
    types.push_back(type);
  }

  bool TypeComponent::has(const std::string &type)
  {
    return count(types.begin(), types.end(), type) > 0;
  }

  InfoComponent::InfoComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    entity = table.get<std::string>("entity");
    variant = table.get<std::string>("variant");

    enable();
  }

  TransformComponent::TransformComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    position = table.get<glm::vec3>("position");
    orientation = glm::quat(glm::radians(table.get<glm::vec3>("orientation")));

    enable();
  }

  EntitySystem::EntitySystem(World *w) :
    System(w)
  {
    components->registerFactory("type", [&](auto id, auto table)
    {
      return std::make_shared<TypeComponent>(id, table);
    });
    components->registerFactory("info", [&](auto id, auto table)
    {
      return std::make_shared<InfoComponent>(id, table);
    });
    components->registerFactory("transform", [&](auto id, auto table)
    {
      return std::make_shared<TransformComponent>(id, table);
    });

    subscribe("set_position", MSGBIND(&EntitySystem::setPosition));
    subscribe("set_orientation", MSGBIND(&EntitySystem::setOrientation));
    subscribe("move", MSGBIND(&EntitySystem::move));
    subscribe("rotate", MSGBIND(&EntitySystem::rotate));
  }

  void EntitySystem::setPosition(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components->getComponent<TransformComponent>(m.getId());
    if (transform)
    {
      transform->position = m.getDirection();
      transform->update = true;
    }
  }

  void EntitySystem::setOrientation(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components->getComponent<TransformComponent>(m.getId());
    if (transform)
    {
      transform->orientation = glm::quat(glm::radians(m.getDirection()));
      transform->update = true;
    }
  }

  void EntitySystem::move(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components->getComponent<TransformComponent>(m.getId());
    if (transform)
    {
      transform->position += m.getDirection();
      transform->update = true;
    }
  }

  void EntitySystem::rotate(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components->getComponent<TransformComponent>(m.getId());
    if (transform)
    {
      transform->orientation *= glm::quat(glm::radians(m.getDirection()));
      transform->update = true;
    }
  }

  void EntitySystem::update(float)
  {
    auto comps = components->getComponents<TransformComponent>();
    for (auto component : comps)
    {
      auto transform = std::static_pointer_cast<TransformComponent>(component.lock());
      if (transform && transform->update)
      {
        Message::MoveActor msg(transform->owner);
        msg.setPosition(transform->position);
        msg.setOrientation(transform->orientation);
        trigger("move_actor", msg);
        transform->update = false;
      }
    }
  }

}
