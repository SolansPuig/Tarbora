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

#include "GameLayer.hpp"
#include "Layer.hpp"
#include "HumanView.hpp"
#include "../../Messages/BasicMessages.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
  GameLayer::GameLayer(HumanView *view, bool start_active) :
    Layer(view, start_active)
  {
    scene_ = std::make_shared<Scene>(view);

    subscribe("create_actor_model", [this](auto &, auto &body)
    {
      Message::CreateActorModel m(body);
      scene_->createActorModel(
        m.getId(), (RenderPass)m.getRenderPass(), m.getModel(), m.getMaterial()
      );
    });

    subscribe("set_camera", [this](auto &, auto &body)
    {
      Message::Node m(body);
      scene_->setCamera(m.getId(), m.getName());
    });

    subscribe("move_actor", [this](auto &, auto &body)
    {
      Message::MoveActor m(body);
      std::shared_ptr<SceneNode> actor = scene_->getActor(m.getId());
      if (actor)
      {
        if (m->parent() != "")
        {
          std::shared_ptr<SceneNode> parent = scene_->getActor(m->parent());
          if (parent)
          {
            std::shared_ptr<SceneNode> node = parent->getChild(m->parent_node());
            if (node)
            {
              auto old_parent = actor->getParent();
              if (old_parent)
              {
                old_parent->removeChild(actor->owner);
              }
              else
              {
                scene_->removeActor(actor->owner);
              }

              node->addChild(actor);
            }
          }
        }

        if (m.hasPosition())
          actor->setPosition(m.getPosition());
        if (m.hasOrientation())
          actor->setOrientation(m.getOrientation());
      }
    });

    subscribe("move_node", [this](auto &, auto &body)
    {
      Message::MoveNode m(body);
      std::shared_ptr<SceneNode> actor = scene_->getActor(m.getId());
      if (actor)
      {
        std::shared_ptr<SceneNode> node = actor->getChild(m.getName());
        if (node)
        {
          if (m.hasPosition())
            node->setPosition(m.getPosition());
          if (m.hasOrientation())
            node->setOrientation(m.getOrientation());
        }
      }
    });

    subscribe("delete_actor_model", [this](auto &, auto &body)
    {
      Message::Actor m(body);
      std::shared_ptr<SceneNode> actor = scene_->getActor(m.getId());
      if (actor)
      {
        auto parent = actor->getParent();
        if (parent)
        {
          parent->removeChild(actor->owner);
        }
        else
        {
          scene_->removeActor(actor->owner);
        }
      }
    });

    subscribe("start_animation", [this](auto &, auto &body)
    {
      Message::StartAnimation m(body);
      auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
      if (actor)
      {
        actor->startAnimation(
          Animation{m.getAnimation(), m.getFile(), (BlendMode)m.getBlendMode(),
                    m.getBlendFactor(), m.getFadeInTimer(), m.getLoop()
          }
        );
      }
    });

    subscribe("start_base_animation", [this](auto &, auto &body)
    {
      Message::StartAnimation m(body);
      auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
      if (actor)
      {
        actor->startAnimation(
          Animation{
            m.getAnimation(), m.getFile(), (BlendMode)m.getBlendMode(),
            m.getBlendFactor(), m.getFadeInTimer(), m.getLoop()
          },
          true
        );
      }
    });

    subscribe("end_animation", [this](auto &, auto &body)
    {
      Message::EndAnimation m(body);
      auto actor = std::static_pointer_cast<ActorModel>(scene_->getActor(m.getId()));
      if (actor)
      {
        actor->endAnimation(
          m.getAnimation(), (StopMode)m.getStopMode(), m.getFadeOutTimer()
        );
      }
    });
  }

  bool GameLayer::getInput()
  {
    return false;
  }

  void GameLayer::update(float delta_time)
  {
    scene_->update(delta_time);
  }

  void GameLayer::draw()
  {
    scene_->draw();
  }
}
