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
    render_pass = table.get<unsigned int>("render_pass", 2);
    model = table.get<std::string>("model", "cube");
    material = table.get<std::string>("material", "white");
  }

  RenderSystem::RenderSystem(World *w) :
    System(w)
  {
    components->registerFactory("model", [&](auto id, auto table)
    {
      return std::make_shared<ModelComponent>(id, table);
    });
    components->onEnable("model", ENBIND(&RenderSystem::enableModel));
    components->onDisable("model", ENBIND(&RenderSystem::disableModel));
  }

  bool RenderSystem::enableModel(std::shared_ptr<Component> comp)
  {
    auto model = std::static_pointer_cast<ModelComponent>(comp);
    auto transform = components->getComponent<TransformComponent>(model->owner);

    if (transform)
    {
      trigger("create_actor_model", Message::CreateActorModel(
                model->owner,
                model->model,
                model->material,
                model->render_pass
              ));

      Message::MoveActor msg(model->owner);

      if (model->parent == "")
      {
        msg.setPosition(transform->position);
        msg.setOrientation(transform->orientation);
      }
      else
      {
        msg->set_parent(model->parent);
        msg->set_parent_node(model->parent_node);
        msg.setPosition(model->offset);
        msg.setOrientation(model->orientation);
      }
      trigger("move_actor", msg);

      return true;
    }
    return false;
  }

  bool RenderSystem::disableModel(std::shared_ptr<Component> comp)
  {
    auto model = std::static_pointer_cast<ModelComponent>(comp);
    trigger("delete_actor_model", Message::Actor(comp->owner));
    return true;
  }

  void RenderSystem::update(float)
  {

  }
}
