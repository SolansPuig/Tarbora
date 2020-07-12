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

#include "Cockatiel.hpp"
#include "../Tarbora/Messages/BasicMessages.hpp"

namespace Tarbora {
  CockatielIA::CockatielIA() :
    Module(23)
  {
    getMessageManager()->subscribe("move_event", [&](auto &, auto &body)
    {
      Message::Event m(body);
      if (m->name() == "grabbed")
      {
        grabbed_ = true;
        scratching_ = false;
      }
      else if (m->name() == "ungrabbed")
      {
        grabbed_ = false;
      }
    });

    getMessageManager()->subscribe("move_actor", [&](auto &, auto &body)
    {
      Message::MoveActor m(body);
      if (m->id() == "cockatiel")
      {
        my_pos_ = m.getPosition();
        my_orientation_ = m.getOrientation();
      }
      else if (m->id() == "player")
      {
        player_pos_ = m.getPosition();
      }

      player_pos_.y = my_pos_.y;

      auto look_qua = glm::quatLookAt(
        glm::normalize(my_pos_ - player_pos_), glm::vec3(0.f, 1.f, 0.f)
      );
      auto direction = look_qua * glm::inverse(my_orientation_);
      auto vector = direction * glm::vec3(0.f, 0.f, 1.f);

      look_lerp_ = 0.f;
      old_direction_ = look_direction_;
      if (vector.z >= glm::radians(20.f) && glm::distance(player_pos_, my_pos_) <= 3.f)
        desired_direction_ = direction;
      else
        desired_direction_ = glm::vec3(0.f);
    });
  }

  CockatielIA::~CockatielIA() {}

  void CockatielIA::update(float delta_time)
  {
    // Look at player or a random point
    if (look_lerp_ < 1.f)
    {
      look_lerp_ += delta_time;
      look_direction_ = glm::mix(old_direction_, desired_direction_, 2 * look_lerp_);
      Message::MoveNode msg("cockatiel", "head");
      msg.setOrientation(look_direction_);
      getMessageManager()->trigger("move_node", msg);
    }

    // Do things
    timer_ += delta_time;
    if (!grabbed_ && scratching_ && timer_ >= 2.f)
    {
      scratching_ = false;
      getMessageManager()->send(
        1, "move_event", Message::Event("cockatiel", "stop_scratching")
      );
    }

    if (!grabbed_ && timer_ >= 3.f)
    {
      timer_ = 0.f;
      int chance = rand() % 101;
      if (chance >= 80)
      {
        scratching_ = true;
        getMessageManager()->send(
          1, "move_event", Message::Event("cockatiel", "scratch")
        );
      }
      else if (chance >= 40)
      {
        if (crest_raised_)
        {
          crest_raised_ = false;
          getMessageManager()->send(
            1, "move_event", Message::Event("cockatiel", "lower_crest")
          );
        }
        else
        {
          crest_raised_ = true;
          getMessageManager()->send(
            1, "move_event", Message::Event("cockatiel", "raise_crest")
          );
        }
      }
    }
  }
}
