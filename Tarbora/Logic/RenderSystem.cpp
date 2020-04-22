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
  ComponentPtr RenderSystem::modelFactory(const ActorId &id, const LuaTable &table)
  {
    auto comp = std::make_shared<ModelComponent>(id);
    comp->render_pass = table.get<int>("render_pass", 1);
    comp->model = table.get<std::string>("model", "cube");
    comp->material = table.get<std::string>("material", "white");

    trigger("create_actor_model", Message::CreateActorModel(
              id,
              comp->model,
              comp->material,
              comp->render_pass
            ));

    auto transform = components_->getComponent<TransformComponent>(id);
    if (transform && transform->enabled())
    {
      Message::MoveActor msg(id);
      msg.setPosition(transform->position);
      msg.setOrientation(transform->orientation);
      trigger("move_actor", msg);
    }

    comp->enable();
    return comp;
  }

  RenderSystem::RenderSystem(World *w) :
    System(w)
  {
    components_->registerFactory("model", FCTBIND(&RenderSystem::modelFactory));
  }

  void RenderSystem::update(float)
  {

  }
}
