#pragma once

#include "MathMessages.hpp"
#include "proto/basic.pb.h"

using namespace tbBasics;

namespace Tarbora {
    class CreateActor : public MessageBody
    {
    public:
        CreateActor(unsigned int id, std::string entity, std::string variant, glm::vec3 position=glm::vec3(), glm::vec3 rotation=glm::vec3())
        {
            CreateActorBody m;
            m.set_id(id);
            m.set_entity(entity);
            m.set_variant(variant);
            (*m.mutable_position()) = Vec3(position);
            (*m.mutable_rotation()) = Vec3(rotation);
            m.SerializeToString(&m_Body);
        }
    };

    class DeleteActor : public MessageBody
    {
    public:
        DeleteActor(unsigned int id)
        {
            DeleteActorBody m;
            m.set_id(id);
            m.SerializeToString(&m_Body);
        }
    };

    class SetCamera : public MessageBody
    {
    public:
        SetCamera(unsigned int actorId, std::string name)
        {
            SetCameraBody m;
            m.set_id(actorId);
            m.set_name(name);
            m.SerializeToString(&m_Body);
        }
    };

    class MoveActor : public MessageBody
    {
    public:
        MoveActor(unsigned int id, glm::vec3 position, glm::mat3 rotation)
        {
            MoveActorBody m;
            m.set_id(id);
            (*m.mutable_position()) = Vec3(position);
            (*m.mutable_rotation()) = Mat3(rotation);
            m.SerializeToString(&m_Body);
        }
    };

    class SetActorAnimation : public MessageBody
    {
    public:
        SetActorAnimation(unsigned int id, std::string animation)
        {
            SetActorAnimationBody m;
            m.set_id(id);
            m.set_animation(animation);
            m.SerializeToString(&m_Body);
        }
    };

    class ApplyPhysics : public MessageBody
    {
    public:
        ApplyPhysics(unsigned int id, float magnitude, glm::vec3 direction)
        {
            ApplyPhysicsBody m;
            m.set_id(id);
            m.set_magnitude(magnitude);
            (*m.mutable_direction()) = Vec3(direction);
            m.SerializeToString(&m_Body);
        }
    };
}
