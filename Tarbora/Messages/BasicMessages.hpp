#pragma once

#include "MathMessages.hpp"
#include "proto/basic.pb.h"

namespace Tarbora {
    namespace Message
    {
        template<class T>
        class Message
        {
        public:
            Message() {}

            Message(const MessageBody *m)
            {
                m_Message.ParseFromString(m->GetContent());
            }

            operator MessageBody()
            {
                std::string body;
                m_Message.SerializeToString(&body);
                return MessageBody(body);
            }

        protected:
            T m_Message;
        };

        class Actor : public Message<::MessageContent::Actor>
        {
        public:
            Actor(const MessageBody *m) : Message<::MessageContent::Actor>(m) {}

            Actor(const ActorId &id)
            {
                m_Message.set_id(id);
            }

            const ActorId &GetId() { return m_Message.id(); }
        };

        class Node : public Message<::MessageContent::Node>
        {
        public:
            Node(const MessageBody *m) : Message<::MessageContent::Node>(m) {}

            Node(const ActorId &id, const std::string &name)
            {
                m_Message.set_id(id);
                m_Message.set_name(name);
            }

            const ActorId &GetId() { return m_Message.id(); }
            const std::string &GetName() { return m_Message.name(); }
        };

        class CreateActor : public Message<::MessageContent::CreateActor>
        {
        public:
            CreateActor(const MessageBody *m) : Message<::MessageContent::CreateActor>(m) {}

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant)
            {
                m_Message.set_id(id);
                m_Message.set_entity(entity);
                m_Message.set_variant(variant);
            }

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const Position &position)
            {
                m_Message.set_id(id);
                m_Message.set_entity(entity);
                m_Message.set_variant(variant);
                (*m_Message.mutable_position()) = Vec3(position.get());
            }

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const Rotation &rotation)
            {
                m_Message.set_id(id);
                m_Message.set_entity(entity);
                m_Message.set_variant(variant);
                (*m_Message.mutable_rotation()) = Vec3(rotation.get());
            }

            CreateActor(const ActorId &id, const std::string &entity, const std::string &variant, const Position &position, const Rotation &rotation)
            {
                m_Message.set_id(id);
                m_Message.set_entity(entity);
                m_Message.set_variant(variant);
                (*m_Message.mutable_position()) = Vec3(position.get());
                (*m_Message.mutable_rotation()) = Vec3(rotation.get());
            }

            const ActorId &GetId() { return m_Message.id(); }
            const std::string &GetEntity() { return m_Message.entity(); }
            const std::string &GetVariant() { return m_Message.variant(); }
            glm::vec3 GetPosition() { return Vec3toGLM(m_Message.position()); }
            glm::vec3 GetRotation() { return Vec3toGLM(m_Message.rotation()); }
            bool HasPosition() { return m_Message.has_position(); }
            bool HasRotation() { return m_Message.has_rotation(); }
        };

        class MoveActor : public Message<::MessageContent::MoveActor>
        {
        public:
            MoveActor(const MessageBody *m) : Message<::MessageContent::MoveActor>(m) {}

            MoveActor(const ActorId &id, const Position &position)
            {
                m_Message.set_id(id);
                (*m_Message.mutable_position()) = Vec3(position.get());
            }

            MoveActor(const ActorId &id, const RotationMat &rotation)
            {
                m_Message.set_id(id);
                (*m_Message.mutable_rotation()) = Mat3(rotation.get());
            }

            MoveActor(const ActorId &id, const Position &position, const RotationMat &rotation)
            {
                m_Message.set_id(id);
                (*m_Message.mutable_position()) = Vec3(position.get());
                (*m_Message.mutable_rotation()) = Mat3(rotation.get());
            }

            const ActorId &GetId() { return m_Message.id(); }
            glm::vec3 GetPosition() { return Vec3toGLM(m_Message.position()); }
            glm::mat3 GetRotation() { return Mat3toGLM(m_Message.rotation()); }
            bool HasPosition() { return m_Message.has_position(); }
            bool HasRotation() { return m_Message.has_rotation(); }
        };

        class MoveNode : public Message<::MessageContent::MoveNode>
        {
        public:
            MoveNode(const MessageBody *m) : Message<::MessageContent::MoveNode>(m) {}

            MoveNode(const ActorId &id, const std::string &name, const Position &position)
            {
                m_Message.set_id(id);
                m_Message.set_name(name);
                (*m_Message.mutable_position()) = Vec3(position.get());
            }

            MoveNode(const ActorId &id, const std::string &name, const Rotation &rotation)
            {
                m_Message.set_id(id);
                m_Message.set_name(name);
                (*m_Message.mutable_rotation()) = Vec3(rotation.get());
            }

            MoveNode(const ActorId &id, const std::string &name, const Position &position, const Rotation &rotation)
            {
                m_Message.set_id(id);
                m_Message.set_name(name);
                (*m_Message.mutable_position()) = Vec3(position.get());
                (*m_Message.mutable_rotation()) = Vec3(rotation.get());
            }

            const ActorId &GetId() { return m_Message.id(); }
            const std::string &GetName() { return m_Message.name(); }
            glm::vec3 GetPosition() { return Vec3toGLM(m_Message.position()); }
            glm::vec3 GetRotation() { return Vec3toGLM(m_Message.rotation()); }
            bool HasPosition() { return m_Message.has_position(); }
            bool HasRotation() { return m_Message.has_rotation(); }
        };

        class SetAnimation : public Message<::MessageContent::SetAnimation>
        {
        public:
            SetAnimation(const MessageBody *m) : Message<::MessageContent::SetAnimation>(m) {}

            SetAnimation(const ActorId &id, const std::string animation)
            {
                m_Message.set_id(id);
                m_Message.set_animation(animation);
            }

            SetAnimation(const ActorId &id, const std::string animation, const std::string file)
            {
                m_Message.set_id(id);
                m_Message.set_animation(animation);
                m_Message.set_file(file);
            }

            SetAnimation(const ActorId &id, const std::string animation, float speed)
            {
                m_Message.set_id(id);
                m_Message.set_animation(animation);
                m_Message.set_speed(speed);
            }

            SetAnimation(const ActorId &id, const std::string animation, const std::string file, float speed)
            {
                m_Message.set_id(id);
                m_Message.set_animation(animation);
                m_Message.set_file(file);
                m_Message.set_speed(speed);
            }

            const ActorId &GetId() { return m_Message.id(); }
            const std::string &GetAnimation() { return m_Message.animation(); }
            const std::string &GetFile() { return m_Message.file(); }
            float GetSpeed() { return m_Message.speed(); }
        };

        class ApplyPhysics : public Message<::MessageContent::ApplyPhysics>
        {
        public:
            ApplyPhysics(const MessageBody *m) : Message<::MessageContent::ApplyPhysics>(m) {}

            ApplyPhysics(const ActorId &id, float magnitude, const Direction &direction)
            {
                m_Message.set_id(id);
                m_Message.set_magnitude(magnitude);
                (*m_Message.mutable_direction()) = Vec3(direction.get());
            }

            const ActorId &GetId() { return m_Message.id(); }
            float GetMagnitude() { return m_Message.magnitude(); }
            glm::vec3 GetDirection() { return Vec3toGLM(m_Message.direction()); }
            bool HasDirection() { return m_Message.has_direction(); }
        };

        class LookAt : public Message<::MessageContent::LookAt>
        {
        public:
            LookAt(const MessageBody *m) : Message<::MessageContent::LookAt>(m) {}

            LookAt(const ActorId &id, const Direction &direction)
            {
                m_Message.set_id(id);
                (*m_Message.mutable_direction()) = Vec3(direction.get());
            }

            LookAt(const ActorId &id, const ActorId &target, float distance, const Direction &direction)
            {
                m_Message.set_id(id);
                m_Message.set_target(target);
                m_Message.set_distance(distance);
                (*m_Message.mutable_direction()) = Vec3(direction.get());
            }

            const ActorId &GetId() { return m_Message.id(); }
            const ActorId &GetTarget() { return m_Message.target(); }
            float GetDistance() { return m_Message.distance(); }
            glm::vec3 GetDirection() { return Vec3toGLM(m_Message.direction()); }
            bool HasDirection() { return m_Message.has_direction(); }
        };
    }
}
