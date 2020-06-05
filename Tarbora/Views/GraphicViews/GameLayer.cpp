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

namespace Tarbora {
  GameLayer::GameLayer(HumanView *view, bool start_active) :
    Layer(view, start_active)
  {
    scene_ = std::shared_ptr<Scene>(new Scene(view));

    movement_ = glm::vec3(0.0f, 0.0f, 0.0f);
    look_direction_ = glm::vec2(0.0f, 0.0f);
    jump_ = false;
    freeze_mouse_ = false;

    std::shared_ptr<Camera> camera = scene_->createCamera("main_camera");
    camera->setRotation(glm::vec3(-1.0f, 180.0f, 0.0f));
    camera->setPosition(glm::vec3(-3.0f, -1.5f, 10.0f));
    scene_->setCamera(camera);

    scene_->createSkybox("sky.mat.lua");

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
                old_parent->removeChild(actor->getActorId());
              }
              else
              {
                scene_->removeActor(actor->getActorId());
              }

              node->addChild(actor);
            }
          }
        }

        if (m.hasPosition())
          actor->setPosition(m.getPosition());
        if (m.hasOrientation())
          actor->setRotation(m.getOrientation());
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
            node->setRotation(m.getOrientation());
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
          parent->removeChild(actor->getActorId());
        }
        else
        {
          scene_->removeActor(actor->getActorId());
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


    scene_->setCamera(target_id_, "1st_person");
  }

  bool GameLayer::onMessage(const MessageBody &body)
  {
    // TODO
    UNUSED(body);
    return false;
  }

  void GameLayer::getInput()
  {
    glm::vec3 last_movement = movement_;

    if (getInputManager()->getKeyDown(KEY_W))
      movement_.z += 1;
    if (getInputManager()->getKeyUp(KEY_W))
      movement_.z -= 1;
    if (getInputManager()->getKeyDown(KEY_S))
      movement_.z -= 1;
    if (getInputManager()->getKeyUp(KEY_S))
      movement_.z += 1;
    if (getInputManager()->getKeyDown(KEY_A))
      movement_.x += 1;
    if (getInputManager()->getKeyUp(KEY_A))
      movement_.x -= 1;
    if (getInputManager()->getKeyDown(KEY_D))
      movement_.x -= 1;
    if (getInputManager()->getKeyUp(KEY_D))
      movement_.x += 1;

    if (getInputManager()->getKeyDown(KEY_X))
    {
      Message::CreateActor msg("", "cube.lua");
      msg.setPosition(glm::vec3(0.f, 1.f, -5.f));
      send(1, "create_actor", msg);
    }
    if (getInputManager()->getKeyDown(KEY_E))
    {
      send(1, "pick_item", Message::Actor(target_id_));
    }
    if (getInputManager()->getKeyDown(KEY_Y))
    {
      static bool third_person = true;
      third_person = !third_person;
      const std::string camera_mode = (third_person ? "3rd_person" : "1st_person");

      LOG_DEBUG("Set camera %s", camera_mode.c_str());
      scene_->setCamera(target_id_, camera_mode);
    }
    if (getInputManager()->getKeyDown(KEY_G))
    {
      pick_object_ = !pick_object_;
           
      if (pick_object_)
        send(1, "grab", Message::Actor(target_id_));
      else
        send(1, "release", Message::Actor(target_id_));
    }
    if (getInputManager()->getButtonDown(MOUSE_BUTTON_1))
    {
      send(1, "cast", Message::Actor(target_id_));
    }
    if (pick_object_ && getInputManager()->mouseScrolled())
    {
      send(1, "grab_distance",
           Message::LookAt(target_id_, getInputManager()->getScrollDelta())
      );
    }

    glm::vec2 last_look_direction_ = look_direction_;
    float sensibility = 0.04; // TODO: Change this to a config file

    if (!freeze_mouse_)
    {
      look_direction_ = sensibility * getInputManager()->getMouseDelta();
    }
    if (movement_ != last_movement)
    {
      send(1, "set_movement",
           Message::ApplyPhysics(target_id_, glm::normalize(movement_))
      );
    }
    if (look_direction_ != last_look_direction_)
    {
      send(1, "set_rotation", Message::ApplyPhysics(
             target_id_,
             glm::vec3(0.f, look_direction_.x, 0.f)
           ));
      send(1, "set_facing", Message::ApplyPhysics(
             target_id_,
             glm::vec3(-look_direction_.y, 0.f, 0.f)
           ));
    }
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
