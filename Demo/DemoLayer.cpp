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

#include "DemoLayer.hpp"
#include "InventoryLayer.hpp"
#include "../Tarbora/Messages/BasicMessages.hpp"

namespace Tarbora {
  DemoLayer::DemoLayer(HumanView *view) :
    GameLayer(view)
  {
    std::shared_ptr<Camera> camera = scene_->createCamera("main_camera");
    camera->setOrientation(glm::vec3(-1.0f, 180.0f, 0.0f));
    camera->setPosition(glm::vec3(-3.0f, -1.5f, 10.0f));
    scene_->setCamera(camera);

    scene_->createSkybox("sky.mat.lua");

    inventory_layer_ = std::make_shared<InventoryLayer>(view_);
    view_->pushLayer(inventory_layer_);

    scene_->setCamera(target_id_, "1st_person");
  }

  bool DemoLayer::getInput()
  {
    // Movement
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

    if (movement_ != last_movement)
      send(1, "set_movement", Message::ApplyPhysics(target_id_, movement_));

    // Look rotation
    glm::vec2 last_look_direction = look_direction_;
    float sensibility = 0.04; // TODO: Change this to a config file

    look_direction_ = sensibility * getInputManager()->getMouseDelta();

    if (look_direction_ != last_look_direction)
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

    // Other actions
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

    if (getInputManager()->getKeyDown(KEY_I))
    {
      inventory_layer_->toggleActive();
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

    return false || GameLayer::getInput();
  }
}
