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

#include "AnimationSystem.hpp"

namespace Tarbora {
  AnimationComponent::AnimationComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    animation_controller_path = table.get<std::string>("file");

    ResourcePtr<LuaScript> res(animation_controller_path);
    if (res != nullptr && start_enabled_)
      enable();
  }

  ComponentPtr AnimationSystem::animationFactory(
    const ActorId &id, const LuaTable &table)
  {
    return std::make_shared<AnimationComponent>(id, table);
  }

  AnimationSystem::AnimationSystem(World *w) :
    System(w)
  {
    components->registerFactory(
      "animation", FCTBIND(&AnimationSystem::animationFactory));

    subscribe("init_event", MSGBIND(&AnimationSystem::init));
    subscribe("move_event", MSGBIND(&AnimationSystem::event));
  }

  void AnimationSystem::init(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m.getId();

    auto anim = components->getComponent<AnimationComponent>(id);

    if (anim && anim->enabled())
    {
      ResourcePtr<LuaScript> res(anim->animation_controller_path);
      if (res != nullptr)
      {
        LuaTable start = res->get("animation_controller").get("start");
        parseEvent(anim.get(), "", start);
        anim->playing_animations.erase("start");
      }
    }
  }

  void AnimationSystem::event(const MessageSubject &, const MessageBody &body)
  {
    Message::Event m(body);
    ActorId id = m.getId();

    auto anim = components->getComponent<AnimationComponent>(id);

    if (anim && anim->enabled())
    {
      ResourcePtr<LuaScript> res(anim->animation_controller_path);
      if (res == nullptr)
        return;

      LuaTable controller = res->get("animation_controller");
      for (auto animation : anim->playing_animations)
      {
        LuaTable event = controller.get(animation).get("events").get(m.getName(), true);
        if (event.valid())
        {
          parseEvent(anim.get(), animation, event);
        }
      }
    }

  }


  void AnimationSystem::parseEvent(
    AnimationComponent *comp, std::string animation, const LuaTable &event)
  {
    // Check if the existing animation should stop
    LuaTable stop = event.get("stop", true);
    if (stop.valid())
    {
      Message::EndAnimation msg(comp->owner, animation);
      msg.setStopMode(stop.get<int>("stop_mode", 0, true));
      msg.setFadeOutTimer(stop.get<float>("fade_out", 0.f, true));
      trigger("end_animation", msg);

      comp->playing_animations.erase(animation);
    }

    // Check what new animations should play
    LuaTable play = event.get("play");
    if (play.valid())
    {
      ResourcePtr<LuaScript> resource(comp->animation_controller_path);
      if (resource == nullptr)
        return;

      LuaTable controller = resource->get("animation_controller");
      for (auto anim : play)
      {
        // Get the new animation
        LuaTable play_anim = anim.second.getAs<LuaTable>();
        std::string name = play_anim.get<std::string>("name");
        LuaTable new_anim = controller.get(name);

        // Trigger the animation event
        Message::StartAnimation msg(
          comp->owner,
          name,
          new_anim.get<std::string>("file")
        );
        msg.setBlendMode(play_anim.get<int>("blend_mode", 0, true));
        msg.setLoop(new_anim.get<bool>("loop", false, true));
        msg.setSpeed(new_anim.get<float>("speed", 1.f, true));
        msg.setBlendFactor(play_anim.get<float>("blend_factor", 1.f, true));
        msg.setFadeInTimer(play_anim.get<float>("fade_in", 0.f, true));
        if (new_anim.get<bool>("base", false, true))
        {
          trigger("start_base_animation", msg);
        }
        else
        {
          trigger("start_animation", msg);
        }
        comp->playing_animations.insert(name);
      }
    }
  }

  void AnimationSystem::update(float)
  {
       
  }
}
