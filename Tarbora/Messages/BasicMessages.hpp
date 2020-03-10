#pragma once

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

        protected:
            T message_;
        };

        class Actor : public Message<::MessageContent::Actor>
        {
        public:
            Actor(const MessageBody &m);
            Actor(const ActorId &id);
            const ActorId& getId();
        };

        class Node : public Message<::MessageContent::Node>
        {
        public:
            Node(const MessageBody &m);
            Node(const ActorId &id, const std::string &name);
            const ActorId& getId();
            const std::string& getName();
        };

        class CreateActor : public Message<::MessageContent::CreateActor>
        {
        public:
            CreateActor(const MessageBody &m);

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant);

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::vec3 &position);

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::quat &rotation);

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const glm::vec3 &position, const glm::quat &rotation);

            const ActorId& getId();
            const std::string& getEntity();
            const std::string& getVariant();
            glm::vec3 getPosition();
            glm::quat getRotation();
            bool hasPosition();
            bool hasRotation();
        };

        class CreateActorModel : public Message<::MessageContent::CreateActorModel>
        {
        public:
            CreateActorModel(const MessageBody &m) : Message<::MessageContent::CreateActorModel>(m) {}

            CreateActorModel(const ActorId &id, const std::string &model, const std::string &material, int render_pass)
            {
                message_.set_id(id);
                message_.set_model(model);
                message_.set_material(material);
                message_.set_render_pass(render_pass);
            }

            const ActorId& getId() { return message_.id(); }
            const std::string& getModel() { return message_.model(); }
            const std::string& getMaterial() { return message_.material(); }
            int getRenderPass() { return message_.render_pass(); }
        };

        class MoveActor : public Message<::MessageContent::MoveActor>
        {
        public:
            MoveActor(const MessageBody &m) : Message<::MessageContent::MoveActor>(m) {}

            MoveActor(const ActorId &id, const glm::vec3 &position)
            {
                message_.set_id(id);
                (*message_.mutable_position()) = toMessage(position);
            }

            MoveActor(const ActorId &id, const glm::quat &rotation)
            {
                message_.set_id(id);
                (*message_.mutable_rotation()) = toMessage(rotation);
            }

            MoveActor(const ActorId &id, const glm::vec3 &position, const glm::quat &rotation)
            {
                message_.set_id(id);
                (*message_.mutable_position()) = toMessage(position);
                (*message_.mutable_rotation()) = toMessage(rotation);
            }

            const ActorId& getId() { return message_.id(); }
            glm::vec3 getPosition() { return toMath(message_.position()); }
            glm::quat getRotation() { return toMath(message_.rotation()); }
            bool hasPosition() { return message_.has_position(); }
            bool hasRotation() { return message_.has_rotation(); }
        };

        class MoveNode : public Message<::MessageContent::MoveNode>
        {
        public:
            MoveNode(const MessageBody &m) : Message<::MessageContent::MoveNode>(m) {}

            MoveNode(const ActorId &id, const std::string &name, const glm::vec3 &position)
            {
                message_.set_id(id);
                message_.set_name(name);
                (*message_.mutable_position()) = toMessage(position);
            }

            MoveNode(const ActorId &id, const std::string &name, const glm::quat &rotation)
            {
                message_.set_id(id);
                message_.set_name(name);
                (*message_.mutable_rotation()) = toMessage(rotation);
            }

            MoveNode(const ActorId &id, const std::string &name, const glm::vec3 &position, const glm::quat &rotation)
            {
                message_.set_id(id);
                message_.set_name(name);
                (*message_.mutable_position()) = toMessage(position);
                (*message_.mutable_rotation()) = toMessage(rotation);
            }

            const ActorId& getId() { return message_.id(); }
            const std::string& getName() { return message_.name(); }
            glm::vec3 getPosition() { return toMath(message_.position()); }
            glm::quat getRotation() { return toMath(message_.rotation()); }
            bool hasPosition() { return message_.has_position(); }
            bool hasRotation() { return message_.has_rotation(); }
        };

        class StartAnimation : public Message<::MessageContent::StartAnimation>
        {
        public:
            StartAnimation(const MessageBody &m) : Message<::MessageContent::StartAnimation>(m) {}

            StartAnimation(const ActorId &id, const std::string animation, const std::string file)
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

            StartAnimation(const ActorId &id, const std::string animation, const std::string file, int blend_mode)
            {
                message_.set_id(id);
                message_.set_animation(animation);
                message_.set_file(file);
                message_.set_speed(1.f);
                message_.set_blend_mode(blend_mode);
                message_.set_blend_factor(1.f);
                message_.set_loop(false);
                message_.set_fade_in_timer(0.f);
            }

            void setSpeed(float speed) { message_.set_speed(speed); }
            void setBlendFactor(float blend_factor) { message_.set_blend_factor(blend_factor); }
            void setLoop(bool loop) { message_.set_loop(loop); }
            void setFadeInTimer(float timer) { message_.set_fade_in_timer(timer); }

            const ActorId& getId() { return message_.id(); }
            const std::string& getAnimation() { return message_.animation(); }
            const std::string& getFile() { return message_.file(); }
            float getSpeed() { return message_.speed(); }
            int getBlendMode() { return message_.blend_mode(); }
            float getBlendFactor() { return message_.blend_factor(); }
            bool getLoop() { return message_.loop(); }
            float getFadeInTimer() { return message_.fade_in_timer(); }
        };

        class EndAnimation : public Message<::MessageContent::EndAnimation>
        {
        public:
            EndAnimation(const MessageBody &m) : Message<::MessageContent::EndAnimation>(m) {}

            EndAnimation(const ActorId &id, const std::string animation)
            {
                message_.set_id(id);
                message_.set_animation(animation);
                message_.set_stop_mode(2);
                message_.set_fade_out_timer(0.f);
            }

            EndAnimation(const ActorId &id, const std::string animation, int stop_mode)
            {
                message_.set_id(id);
                message_.set_animation(animation);
                message_.set_stop_mode(stop_mode);
                message_.set_fade_out_timer(0.f);
            }

            void setFadeOutTimer(float timer) { message_.set_fade_out_timer(timer); }

            const ActorId& getId() { return message_.id(); }
            const std::string& getAnimation() { return message_.animation(); }
            int getStopMode() { return message_.stop_mode(); }
            float getFadeOutTimer() { return message_.fade_out_timer(); }
        };

        class ApplyPhysics : public Message<::MessageContent::ApplyPhysics>
        {
        public:
            ApplyPhysics(const MessageBody &m) : Message<::MessageContent::ApplyPhysics>(m) {}

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
            LookAt(const MessageBody &m) : Message<::MessageContent::LookAt>(m) {}

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

            const ActorId& getId() { return message_.id(); }
            const ActorId& getTarget() { return message_.target(); }
            float getDistance() { return message_.distance(); }
            glm::vec3 getDirection() { return toMath(message_.direction()); }
            bool hasDirection() { return message_.has_direction(); }
        };
    }
}
