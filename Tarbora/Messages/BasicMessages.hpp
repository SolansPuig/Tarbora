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

#ifndef __BASIC_MESSAGES_H_
#define __BASIC_MESSAGES_H_

#include "MathMessages.hpp"
#include "basic.pb.h"

namespace Tarbora {
  namespace Message
  {
    template<class T>
    class Message
    {
    public:
      Message() {}

      Message(const MessageBody &m)
      {
        message_.ParseFromString(m.getContent());
      }

      operator MessageBody()
      {
        std::string body;
        message_.SerializeToString(&body);
        return MessageBody(body);
      }

      T* operator->()
      {
        return &message_;
      }

    protected:
      T message_;
    };

    class Actor : public Message<::MessageContent::Actor>
    {
    public:
      Actor(const MessageBody &m) :
        Message<::MessageContent::Actor>(m) {}

      Actor(const ActorId &id)
      {
        message_.set_id(id);
      }

      inline const ActorId& getId() { return message_.id(); }
    };

    class Node : public Message<::MessageContent::Node>
    {
    public:
      Node(const MessageBody &m) :
        Message<::MessageContent::Node>(m) {}

      Node(const ActorId &id, const std::string &name)
      {
        message_.set_id(id);
        message_.set_name(name);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getName() { return message_.name(); }
    };

    class CreateActor : public Message<::MessageContent::CreateActor>
    {
    public:
      CreateActor(const MessageBody &m) :
        Message<::MessageContent::CreateActor>(m) {}

      CreateActor(const ActorId &id, const std::string &entity)
      {
        message_.set_id(id);
        message_.set_entity(entity);
      }

      inline void setVariant(const std::string &variant)
      {
        message_.set_variant(variant);
      }

      inline void setPosition(const glm::vec3 &position)
      {
        (*message_.mutable_position()) = toMessage(position);
      }

      void setOrientation(const glm::quat &orientation)
      {
        (*message_.mutable_orientation()) = toMessage(orientation);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getEntity() { return message_.entity(); }
      inline const std::string& getVariant() { return message_.variant(); }
      inline glm::vec3 getPosition() { return toMath(message_.position()); }
      inline glm::quat getOrientation() { return toMath(message_.orientation()); }
      inline bool hasPosition() { return message_.has_position(); }
      inline bool hasOrientation() { return message_.has_orientation(); }
    };

    class CreateActorModel : public Message<::MessageContent::CreateActorModel>
    {
    public:
      CreateActorModel(const MessageBody &m) :
        Message<::MessageContent::CreateActorModel>(m) {}

      CreateActorModel(
        const ActorId &id,
        const std::string &model,
        const std::string &material,
        int render_pass
      )
      {
        message_.set_id(id);
        message_.set_model(model);
        message_.set_material(material);
        message_.set_render_pass(render_pass);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getModel() { return message_.model(); }
      inline const std::string& getMaterial() { return message_.material(); }
      inline int getRenderPass() { return message_.render_pass(); }
    };

    class MoveActor : public Message<::MessageContent::MoveActor>
    {
    public:
      MoveActor(const MessageBody &m) :
        Message<::MessageContent::MoveActor>(m) {}

      MoveActor(const ActorId &id)
      {
        message_.set_id(id);
      }

      inline void setPosition(const glm::vec3 &position)
      {
        (*message_.mutable_position()) = toMessage(position);
      }

      inline void setOrientation(const glm::quat &orientation)
      {
        (*message_.mutable_orientation()) = toMessage(orientation);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline glm::vec3 getPosition() { return toMath(message_.position()); }
      inline glm::quat getOrientation() { return toMath(message_.orientation()); }
      inline bool hasPosition() { return message_.has_position(); }
      inline bool hasOrientation() { return message_.has_orientation(); }
    };

    class MoveNode : public Message<::MessageContent::MoveNode>
    {
    public:
      MoveNode(const MessageBody &m) :
        Message<::MessageContent::MoveNode>(m) {}

      MoveNode(const ActorId &id, const std::string &name)
      {
        message_.set_id(id);
        message_.set_name(name);
      }

      inline void setPosition(const glm::vec3 &position)
      {
        (*message_.mutable_position()) = toMessage(position);
      }

      inline void setOrientation(const glm::quat &orientation)
      {
        (*message_.mutable_orientation()) = toMessage(orientation);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getName() { return message_.name(); }
      inline glm::vec3 getPosition() { return toMath(message_.position()); }
      inline glm::quat getOrientation() { return toMath(message_.orientation()); }
      inline bool hasPosition() { return message_.has_position(); }
      inline bool hasOrientation() { return message_.has_orientation(); }
    };

    class StartAnimation : public Message<::MessageContent::StartAnimation>
    {
    public:
      StartAnimation(const MessageBody &m) :
        Message<::MessageContent::StartAnimation>(m) {}

      StartAnimation(
        const ActorId &id,
        const std::string animation,
        const std::string file
      )
      {
        message_.set_id(id);
        message_.set_animation(animation);
        message_.set_file(file);
        message_.set_speed(1.f);
        message_.set_blend_mode(0);
        message_.set_blend_factor(1.f);
        message_.set_loop(false);
        message_.set_fade_in_timer(0.f);
      }

      inline void setBlendMode(int blend_mode)
      {
        message_.set_blend_mode(blend_mode);
      }

      inline void setSpeed(float speed)
      {
        message_.set_speed(speed);
      }

      inline void setBlendFactor(float blend_factor)
      {
        message_.set_blend_factor(blend_factor);
      }

      inline void setLoop(bool loop)
      {
        message_.set_loop(loop);
      }

      inline void setFadeInTimer(float timer)
      {
        message_.set_fade_in_timer(timer);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getAnimation() { return message_.animation(); }
      inline const std::string& getFile() { return message_.file(); }
      inline float getSpeed() { return message_.speed(); }
      inline int getBlendMode() { return message_.blend_mode(); }
      inline float getBlendFactor() { return message_.blend_factor(); }
      inline bool getLoop() { return message_.loop(); }
      inline float getFadeInTimer() { return message_.fade_in_timer(); }
    };

    class EndAnimation : public Message<::MessageContent::EndAnimation>
    {
    public:
      EndAnimation(const MessageBody &m) :
        Message<::MessageContent::EndAnimation>(m) {}

      EndAnimation(const ActorId &id, const std::string animation)
      {
        message_.set_id(id);
        message_.set_animation(animation);
        message_.set_stop_mode(2);
        message_.set_fade_out_timer(0.f);
      }

      inline void setStopMode(int stop_mode)
      {
        message_.set_stop_mode(stop_mode);
      }

      inline void setFadeOutTimer(float timer)
      {
        message_.set_fade_out_timer(timer);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getAnimation() { return message_.animation(); }
      inline int getStopMode() { return message_.stop_mode(); }
      inline float getFadeOutTimer() { return message_.fade_out_timer(); }
    };

    class ApplyPhysics : public Message<::MessageContent::ApplyPhysics>
    {
    public:
      ApplyPhysics(const MessageBody &m) :
        Message<::MessageContent::ApplyPhysics>(m) {}

      ApplyPhysics(const ActorId &id, const glm::vec3 &direction)
      {
        message_.set_id(id);
        (*message_.mutable_direction()) = toMessage(direction);
      }

      const ActorId& getId() { return message_.id(); }
      glm::vec3 getDirection() { return toMath(message_.direction()); }
      bool hasDirection() { return message_.has_direction(); }
    };

    class LookAt : public Message<::MessageContent::LookAt>
    {
    public:
      LookAt(const MessageBody &m) :
        Message<::MessageContent::LookAt>(m) {}

      LookAt(const ActorId &id, const glm::vec3 &direction)
      {
        message_.set_id(id);
        (*message_.mutable_direction()) = toMessage(direction);
      }

      LookAt(const ActorId &id, const ActorId &target)
      {
        message_.set_id(id);
        message_.set_target(target);
      }

      LookAt(const ActorId &id, const ActorId &target, float distance)
      {
        message_.set_id(id);
        message_.set_target(target);
        message_.set_distance(distance);
      }

      LookAt(const ActorId &id, float distance)
      {
        message_.set_id(id);
        message_.set_distance(distance);
      }

      void setPosition(const glm::vec3 &position)
      {
        (*message_.mutable_position()) = toMessage(position);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const ActorId& getTarget() { return message_.target(); }
      inline float getDistance() { return message_.distance(); }
      inline glm::vec3 getDirection() { return toMath(message_.direction()); }
      inline bool hasDirection() { return message_.has_direction(); }
    };

    class Event : public Message<::MessageContent::Event>
    {
    public:
      Event(const MessageBody &m) :
        Message<::MessageContent::Event>(m) {}

      Event(const ActorId &id, const std::string &name)
      {
        message_.set_id(id);
        message_.set_name(name);
      }

      inline const ActorId& getId() { return message_.id(); }
      inline const std::string& getName() { return message_.name(); }
    };

    class Inventory : public Message<::MessageContent::Inventory>
    {
    public:
      Inventory(const MessageBody &m) : Message<::MessageContent::Inventory>(m) {}
      Inventory(const ActorId &id) { message_.set_id(id); }
    };

    class Item : public Message<::MessageContent::Item>
    {
    public:
      Item(const MessageBody &m) :
        Message<::MessageContent::Item>(m) {}

      Item(const ActorId &id, const std::string &group,
           unsigned int slot, unsigned int quantity)
      {
        message_.set_owner(id);
        message_.set_inv_group(group);
        message_.set_slot(slot);
        message_.set_quantity(quantity);
      }
    };
  }
}

#endif // __BASICMESSAGES_H_
