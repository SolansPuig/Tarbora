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

namespace Tarbora {
  World::World() :
    Module(1)
  {
    max_fps_ = 60;

    getMessageManager()->subscribe("create_actor", [&](auto, auto body)
    {
      Message::CreateActor m(body);
      createActor(m->id(), m->entity(), m.getPosition(),  m.getOrientation());
    });
  }

  ActorId World::createActor(
    const ActorId &id,
    const std::string &entity,
    const glm::vec3 &position,
    const glm::quat &orientation
  )
  {
    ResourcePtr<LuaScript> resource("entities/" + entity);
    if (resource == nullptr)
      return "";

    //  If it doesn't have an id, set it as a unique number
    ActorId new_id = id;
    if (new_id == "")
      new_id = std::to_string(next_id_++);


    // Everything has an info component
    LuaTable info = resource->createTable("info");
    info.set<std::string>("entity", entity);
    info.set<std::string>("variant", "");
    components_.createComponent(new_id, "info", info);

    // Everything has a transform component
    glm::vec3 orientation_euler = glm::degrees(glm::eulerAngles(orientation));
    LuaTable transform = resource->createTable("transform");
    transform.set<glm::vec3>("position", position);
    transform.set<glm::vec3>("orientation", orientation_euler);
    components_.createComponent(new_id, "transform", transform);

    // Create the other components
    LuaTable components = resource->get("components", true);
    for (auto component : components)
    {
      std::string name = component.first.getAs<std::string>();
      LuaTable value = component.second.getAs<LuaTable>();
      components_.createComponent(new_id, name, value);
    }

    enableActor(new_id);
    return new_id;
  }

  void World::deleteActor(const ActorId &id)
  {
    disableActor(id);
    components_.deleteEntityComponents(id);
  }

  void World::enableActor(const ActorId &id)
  {
    components_.enableEntityComponents(id);
  }

  void World::disableActor(const ActorId &id)
  {
    components_.disableEntityComponents(id);
  }

  void World::update(float delta_time)
  {
    for (auto itr = systems_.rbegin(); itr != systems_.rend(); itr++)
    {
      (*itr)->update(delta_time);
    }

    components_.clean();
  }
}
