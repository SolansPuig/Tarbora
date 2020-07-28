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

#include "AnimationController.hpp"
#include "ActorModel.hpp"

namespace Tarbora {
  AnimationController::AnimationController(ActorModel *actor) :
    actor_model_(actor)
  {
    global_timer_ = 0.f;
  }

  void AnimationController::update(float delta_time)
  {
    global_timer_ += delta_time;
    if (global_timer_ >  0.016f)
    {
      for (auto it = animations_.begin(); it != animations_.end(); it++)
        if (!it->update(global_timer_, actor_model_))
        {
          it = animations_.erase(it);
          --it;
        }

      global_timer_ = 0.f;
    }
  }

  void AnimationController::startAnimation(Animation animation, bool background)
  {
    animation.timer = 0.f;

    if (background)
      animations_.push_front(animation);
    else
      animations_.push_back(animation);
  }

  void AnimationController::endAnimation(
    const std::string &name, StopMode mode, float fade_out_timer
  )
  {
    auto it = std::find_if(animations_.begin(), animations_.end(), [&](const Animation a) {
      return a.name == name;
    });

    if (it != animations_.end())
    {
      switch (mode)
      {
        case Now:
          animations_.erase(it);
          break;
        case EndLoop:
          it->loop = false;
          break;
        case FadeOut:
          it->fade_out_timer = fade_out_timer;
          if (it->blend_mode == BlendMode::Override)
            it->blend_mode = BlendMode::Mix;
          break;
        case EndLoopFadingOut:
          it->loop = false;
          it->fade_out_timer = it->duration - it->timer;
          if (it->blend_mode == BlendMode::Override)
            it->blend_mode = BlendMode::Mix;
          break;
      }
    }
  }

  bool Animation::update(float delta_time, ActorModel *actor)
  {
    ResourcePtr<LuaScript> resource("animations/" + file);
    LuaTable animation = resource->get(name, true);

    if (fade_in_timer > 0.f)
      fade_in_timer -= delta_time;

    if (fade_out_timer >= 0.f)
    {
      fade_out_timer -= delta_time;
      if (fade_out_timer < 0.f)
      {
        return false;
      }
    }

    if (animation.valid())
    {
      LuaTable nodes = animation.get("nodes");
      duration = animation.get<float>("duration");
      float step = animation.get<float>("step", 1, true);
      float current_frame = timer / step;

      LuaTable query = resource->get("query");
      query.set("time", timer);

      for (auto itr : nodes)
      {
        std::string node_name = itr.first.getAs<std::string>();
        LuaTable node_lua = itr.second.getAs<LuaTable>();
        std::shared_ptr<SceneNode> n = actor->nodes_[node_name];

        if (n->getType() == "animated")
        {
          auto node = std::static_pointer_cast<AnimatedNode>(n);

          for (auto property : node_lua)
          {
            std::string name = property.first.getAs<std::string>();
            glm::vec3 new_value(0.f);
            glm::quat new_quat(1.f, 0.f, 0.f, 0.f);

            if (property.second.is<LuaFunction<LuaTable>>())
            {
              if (name == "global_scale")
                new_value.x = property.second.getAs<float>();
              else if (name == "rotation")
                new_quat = glm::quat(glm::radians(property.second.getAs<glm::vec3>()));
              else
                new_value = property.second.getAs<glm::vec3>();
            }
            else
            {
              LuaTable keyframes = property.second.getAs<LuaTable>();

              int previous_frame = 0;
              int next_frame = 0;
              for (auto keyframe : keyframes)
              {
                int frame = keyframe.first.getAs<int>();
                if (frame <= current_frame)
                  previous_frame = frame;
                else
                {
                  next_frame = frame;
                  break;
                }
              }
              float distance = (next_frame > 0 ?
                                (next_frame - previous_frame) :
                                (duration - previous_frame));
              float lerp_factor = (current_frame - previous_frame)/
                (distance > 0 ? distance : 1);

              if (name == "global_scale")
              {
                float previous_value = keyframes.get<float>(previous_frame);
                float next_value = keyframes.get<float>(next_frame);
                new_value.x = glm::lerp(previous_value, next_value, lerp_factor);
              }
              else if (name == "rotation")
              {
                auto previous_value = keyframes.get<glm::vec3>(previous_frame);
                auto next_value = keyframes.get<glm::vec3>(next_frame);
                new_quat = glm::mix(
                  glm::quat(glm::radians(previous_value)),
                  glm::quat(glm::radians(next_value)),
                  lerp_factor);
              }
              else
              {
                auto previous_value = keyframes.get<glm::vec3>(previous_frame);
                auto next_value = keyframes.get<glm::vec3>(next_frame);
                new_value = glm::lerp(previous_value, next_value, lerp_factor);
              }
            }

            if (name == "position")
            {
              auto old_value = node->getPositionAnimation();
              auto value = blendProperty(new_value/100.f, old_value, this);
              node->setPositionAnimation(value);
            }
            else if (name == "rotation")
            {
              auto old_value = node->getOrientationAnimation();
              auto value = blendProperty(new_quat, old_value, this);
              node->setOrientationAnimation(value);
            }
            else if (name == "scale")
            {
              auto old_value = node->getScaleAnimation();
              auto value = blendProperty(new_value/100.f, old_value, this);
              node->setScaleAnimation(value);
            }
            else if (name == "global_scale")
            {
              auto old_value = node->getGlobalScaleAnimation();
              auto value = blendProperty(new_value.x, old_value, this);
              node->setGlobalScaleAnimation(value);
            }
            else if (name == "uv_map")
            {
              auto old_value = glm::vec3(node->getUvMapAnimation(), 0.f);
              auto value = blendProperty(new_value, old_value, this);
              node->setUvMapAnimation(value);
            }
            else if (name == "color_primary")
            {
              glm::vec3 old_value = node->getColorPrimaryAnimation();
              auto value = blendProperty(new_value/255.f, old_value, this);
              node->setColorPrimaryAnimation(value);
            }
            else if (name == "color_secondary")
            {
              glm::vec3 old_value = node->getColorSecondaryAnimation();
              auto value = blendProperty(new_value/255.f, old_value, this);
              node->setColorSecondaryAnimation(value);
            }
            else if (name == "color_detail")
            {
              glm::vec3 old_value = node->getColorDetailAnimation();
              auto value = blendProperty(new_value/255.f, old_value, this);
              node->setColorDetailAnimation(value);
            }
            else if (name == "color_emissive")
            {
              glm::vec3 old_value = node->getColorEmissiveAnimation();
              auto value = blendProperty(new_value/255.f, old_value, this);
              node->setColorEmissiveAnimation(value);
            }
          }
        }
      }

      timer += delta_time;
      if (current_frame >= duration)
      {
        if (loop)
          timer -= duration * step;
        else
          return false;
      }
      return true;
    }
    return false;
  }
}
