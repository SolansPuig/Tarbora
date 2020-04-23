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

#include "World.hpp"
#include "../Messages/BasicMessages.hpp"
#include "EntitySystem.hpp"
#include "PhysicsSystem.hpp"
#include "ControllerSystem.hpp"
#include "RenderSystem.hpp"
#include "AnimationSystem.hpp"

namespace Tarbora {
  World::World() :
    Module(1), entity_system_(this), render_system_(this), animation_system_(this),
    controller_system_(this), physics_system_(this)
  {
    max_fps_ = 60;

    getMessageManager()->subscribe("create_actor", MSGBIND(&World::createActor));
  }

  void World::createActor(const MessageSubject &, const MessageBody &body)
  {
    Message::CreateActor m(body);
    ActorId id = m.getId();
    std::string entity = m.getEntity();

    ResourcePtr<LuaScript> resource("entities/" + entity);
    if (resource == nullptr)
      return;

    glm::vec3 position = m.getPosition();
    glm::vec3 orientation = glm::degrees(glm::eulerAngles(m.getOrientation()));

    //  If it doesn't have an id, set is as a unique number
    if (id == "")
      id = std::to_string(next_id_++);

    // Everything has an info component
    LuaTable info = resource->createTable("info");
    info.set<std::string>("entity", entity);
    info.set<std::string>("variant", "");
    components_.createComponent(id, "info", info);

    // Everything has a transform component
    LuaTable transform = resource->createTable("transform");
    transform.set<glm::vec3>("position", position);
    transform.set<glm::vec3>("orientation", orientation);
    components_.createComponent(id, "transform", transform);

    // Create the other components
    LuaTable components = resource->get("components", true);
    for (auto component : components)
    {
      std::string name = component.first.getAs<std::string>();
      LuaTable value = component.second.getAs<LuaTable>();
      components_.createComponent(id, name, value);
    }

    getMessageManager()->triggerLocal("init_event", Message::Actor(id));
  }

  void World::update(float delta_time)
  {
    controller_system_.update(delta_time);
    physics_system_.update(delta_time);
    render_system_.update(delta_time);
    animation_system_.update(delta_time);
    entity_system_.update(delta_time);
  }
}
