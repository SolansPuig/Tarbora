#include "../inc/BasicMessages.hpp"
#include "../inc/MessageManager.hpp"
#include "../../ClientApplication.hpp"

namespace Tarbora {
    void CreateActor(ClientApplication *app, std::string entity, std::string variant, std::string position, std::string rotation)
    {
        std::string str;

        CreateActorMessage m;
        m.set_entity(entity);
        m.set_variant(variant);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Vec3(rotation);

        m.SerializeToString(&str);

        app->MessageManager()->Send(1, "create_actor", str);
    }

    void CreateActor(ClientApplication *app, std::string entity, std::string variant, glm::vec3 position, glm::vec3 rotation)
    {
        std::string str;

        CreateActorMessage m;
        m.set_entity(entity);
        m.set_variant(variant);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Vec3(rotation);

        m.SerializeToString(&str);

        app->MessageManager()->Send(1, "create_actor", str);
    }

    void CreateActorModel(ClientApplication *app, unsigned int id, std::string entity, std::string variant)
    {
        std::string str;

        CreateActorMessage m;
        m.set_id(id);
        m.set_entity(entity);
        m.set_variant(variant);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("create_actor_model", str);
    }

    void DeleteActor(ClientApplication *app, unsigned int id)
    {
        std::string str;

        DeleteActorMessage m;
        m.set_id(id);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("delete_actor", str);
    }

    void MoveActor(ClientApplication *app, unsigned int id, glm::vec3 position, glm::mat3 rotation)
    {
        std::string str;

        MoveActorMessage m;
        m.set_id(id);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Mat3(rotation);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("move_actor", str);
    }

    void MoveActor(ClientApplication *app, unsigned int id, std::string position, std::string rotation)
    {
        std::string str;

        MoveActorMessage m;
        m.set_id(id);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Mat3(rotation);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("move_actor", str);
    }

    void AnimateActor(ClientApplication *app, unsigned int id, std::string animation)
    {
        std::string str;

        AnimateActorMessage m;
        m.set_id(id);
        m.set_animation(animation);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("animate_actor", str);
    }

    void ApplyForce(ClientApplication *app, unsigned int id, float magnitude, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("apply_force", str);
    }

    void ApplyForce(ClientApplication *app, unsigned int id, float magnitude, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("apply_force", str);
    }

    void ApplyTorque(ClientApplication *app, unsigned int id, float magnitude, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("apply_torque", str);
    }

    void ApplyTorque(ClientApplication *app, unsigned int id, float magnitude, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("apply_torque", str);
    }

    void SetVelocity(ClientApplication *app, unsigned int id, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("set_velocity", str);
    }

    void SetVelocity(ClientApplication *app, unsigned int id, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("set_velocity", str);
    }

    void Stop(ClientApplication *app, unsigned int id)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);

        m.SerializeToString(&str);

        app->MessageManager()->Trigger("stop", str);
    }
}
