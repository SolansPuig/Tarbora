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

#include "RenderSystem.hpp"
#include "EntitySystem.hpp"

namespace Tarbora {
  ModelComponent::ModelComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    render_pass = table.get<int>("render_pass", 1);
    model = table.get<std::string>("model", "cube");
    material = table.get<std::string>("material", "white");

    if (start_enabled_)
      enabled_ = true;
  }

  ComponentPtr RenderSystem::modelFactory(const ActorId &id, const LuaTable &table)
  {
    return std::make_shared<ModelComponent>(id, table);
  }

  RenderSystem::RenderSystem(World *w) :
    System(w)
  {
    components->registerFactory("model", FCTBIND(&RenderSystem::modelFactory));

    subscribe("init_event", MSGBIND(&RenderSystem::init));
  }

  void RenderSystem::init(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m.getId();

    auto model = components->getComponent<ModelComponent>(id);
    auto transform = components->getComponent<TransformComponent>(id);

    if (model && model->enabled() && transform && transform->enabled())
    {
      trigger("create_actor_model", Message::CreateActorModel(
                id,
                model->model,
                model->material,
                model->render_pass
              ));

      Message::MoveActor msg(id);
      msg.setPosition(transform->position);
      msg.setOrientation(transform->orientation);
      trigger("move_actor", msg);
    }
    else
    {
      LOG_WARN("Entity %s has a model but no transform component.", id.c_str());
    }
  }

  void RenderSystem::update(float)
  {

  }
}
