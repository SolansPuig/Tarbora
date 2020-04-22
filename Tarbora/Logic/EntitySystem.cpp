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
  ComponentPtr EntitySystem::typeFactory(const ActorId &id, const LuaTable &table)
  {
    auto comp = std::make_shared<TypeComponent>(id);
    for (auto type : table)
    {
      comp->add(type.second.getAs<std::string>());
    }

    comp->enable();
    return comp;
  }

  void TypeComponent::add(const std::string &type)
  {
    types.push_back(type);
  }

  bool TypeComponent::has(const std::string &type)
  {
    return count(types.begin(), types.end(), type) > 0;
  }

  ComponentPtr EntitySystem::infoFactory(const ActorId &id, const LuaTable &table)
  {
    auto comp = std::make_shared<InfoComponent>(id);
    comp->entity = table.get<std::string>("entity");
    comp->variant = table.get<std::string>("variant");

    comp->enable();
    return comp;
  }

  ComponentPtr EntitySystem::transformFactory(const ActorId &id, const LuaTable &table)
  {
    auto comp = std::make_shared<TransformComponent>(id);
    comp->position = table.get<glm::vec3>("position");
    comp->orientation = glm::quat(glm::radians(table.get<glm::vec3>("orientation")));

    comp->enable();
    return comp;
  }

  EntitySystem::EntitySystem(World *w) :
    System(w)
  {
    components_->registerFactory("type", FCTBIND(&EntitySystem::typeFactory));
    components_->registerFactory("info", FCTBIND(&EntitySystem::infoFactory));
    components_->registerFactory("transform", FCTBIND(&EntitySystem::transformFactory));

    subscribe("set_position", MSGBIND(&EntitySystem::setPosition));
    subscribe("set_orientation", MSGBIND(&EntitySystem::setOrientation));
    subscribe("move", MSGBIND(&EntitySystem::move));
    subscribe("rotate", MSGBIND(&EntitySystem::rotate));
  }

  void EntitySystem::setPosition(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components_->getComponent<TransformComponent>(m.getId());
    if (transform && transform->enabled())
    {
      transform->position = m.getDirection();
      transform->update = true;
    }
  }

  void EntitySystem::setOrientation(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components_->getComponent<TransformComponent>(m.getId());
    if (transform && transform->enabled())
    {
      transform->orientation = glm::quat(glm::radians(m.getDirection()));
      transform->update = true;
    }
  }

  void EntitySystem::move(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components_->getComponent<TransformComponent>(m.getId());
    if (transform && transform->enabled())
    {
      transform->position += m.getDirection();
      transform->update = true;
    }
  }

  void EntitySystem::rotate(const MessageSubject &, const MessageBody &body)
  {
    Message::ApplyPhysics m(body);
    auto transform = components_->getComponent<TransformComponent>(m.getId());
    if (transform && transform->enabled())
    {
      transform->orientation *= glm::quat(glm::radians(m.getDirection()));
      transform->update = true;
    }
  }

  void EntitySystem::update(float)
  {
    auto comps = components_->getComponents<TransformComponent>();
    for (auto component : comps)
    {
      auto transform = std::static_pointer_cast<TransformComponent>(component);
      if (transform->enabled() && transform->update)
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
