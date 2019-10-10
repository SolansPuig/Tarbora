#pragma once

#include "MathMessages.hpp"
#include "proto/basic.pb.h"

using namespace tbBasics;

namespace Tarbora {
    class CreateActor : public MessageBody
    {
    public:
        CreateActor(ActorId id, std::string entity, std::string variant, glm::vec3 position=glm::vec3(), glm::vec3 rotation=glm::vec3())
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
        DeleteActor(ActorId id)
        {
            DeleteActorBody m;
            m.set_id(id);
            m.SerializeToString(&m_Body);
        }
    };

    class SetCamera : public MessageBody
    {
    public:
        SetCamera(ActorId id, std::string name)
        {
            SetCameraBody m;
            m.set_id(id);
            m.set_name(name);
            m.SerializeToString(&m_Body);
        }
    };

    class MoveActor : public MessageBody
    {
    public:
        MoveActor(ActorId id, glm::vec3 position, glm::mat3 rotation)
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
        SetActorAnimation(ActorId id, std::string animation)
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
        ApplyPhysics(ActorId id, float magnitude, glm::vec3 direction)
        {
            ApplyPhysicsBody m;
            m.set_id(id);
            m.set_magnitude(magnitude);
            (*m.mutable_direction()) = Vec3(direction);
            m.SerializeToString(&m_Body);
        }
    };

    class LookDirection : public MessageBody
    {
    public:
        LookDirection(ActorId id, glm::vec2 direction)
        {
            LookDirectionBody m;
            m.set_id(id);
            (*m.mutable_direction()) = Vec2(direction);
            m.SerializeToString(&m_Body);
        }
    };

    class LookAt : public MessageBody
    {
    public:
        LookAt(ActorId id, ActorId target, float distance, glm::vec3 rotation)
        {
            LookAtBody m;
            m.set_id(id);
            m.set_target(target);
            m.set_distance(distance);
            (*m.mutable_rotation()) = Vec3(rotation);
            m.SerializeToString(&m_Body);
        }
    };

    class MoveNode : public MessageBody
    {
    public:
        MoveNode(ActorId id, std::string nodeName, glm::vec3 position, glm::vec3 rotation)
        {
            MoveNodeBody m;
            m.set_id(id);
            m.set_node(nodeName);
            (*m.mutable_position()) = Vec3(position);
            (*m.mutable_rotation()) = Vec3(rotation);
            m.SerializeToString(&m_Body);
        }
    };

    class Matrix : public MessageBody
    {
    public:
        Matrix(glm::mat4 matrix)
        {
            MatrixBody m;
            (*m.mutable_matrix()) = Mat4(matrix);
            m.SerializeToString(&m_Body);
        }
    };
}
