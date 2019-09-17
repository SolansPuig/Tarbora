#include "../inc/BasicMessages.hpp"
#include "../inc/MessageManager.hpp"
#include "../../Module.hpp"

namespace Tarbora {
    void CreateActor(Module *app, std::string entity, std::string variant, std::string position, std::string rotation)
    {
        std::string str;

        CreateActorMessage m;
        m.set_entity(entity);
        m.set_variant(variant);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Vec3(rotation);

        m.SerializeToString(&str);

        app->GetMessageManager()->Send(1, "create_actor", str);
    }

    void CreateActor(Module *app, std::string entity, std::string variant, glm::vec3 position, glm::vec3 rotation)
    {
        std::string str;

        CreateActorMessage m;
        m.set_entity(entity);
        m.set_variant(variant);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Vec3(rotation);

        m.SerializeToString(&str);

        app->GetMessageManager()->Send(1, "create_actor", str);
    }

    void CreateActorModel(Module *app, unsigned int id, std::string entity, std::string variant)
    {
        std::string str;

        CreateActorMessage m;
        m.set_id(id);
        m.set_entity(entity);
        m.set_variant(variant);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("create_actor_model", str);
    }

    void DeleteActor(Module *app, unsigned int id)
    {
        std::string str;

        DeleteActorMessage m;
        m.set_id(id);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("delete_actor", str);
    }

    void SetCamera(Module *app, unsigned int actorId, std::string name)
    {
        LOG_DEBUG("hi");
        std::string str;

        SetCameraMessage m;
        m.set_id(actorId);
        m.set_name(name);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("set_camera", str);
        LOG_DEBUG("send");
    }

    void MoveActor(Module *app, unsigned int id, glm::vec3 position, glm::mat3 rotation)
    {
        std::string str;

        MoveActorMessage m;
        m.set_id(id);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Mat3(rotation);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("move_actor", str);
    }

    void MoveActor(Module *app, unsigned int id, std::string position, std::string rotation)
    {
        std::string str;

        MoveActorMessage m;
        m.set_id(id);
        (*m.mutable_position()) = Vec3(position);
        (*m.mutable_rotation()) = Mat3(rotation);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("move_actor", str);
    }

    void AnimateActor(Module *app, unsigned int id, std::string animation)
    {
        std::string str;

        AnimateActorMessage m;
        m.set_id(id);
        m.set_animation(animation);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("animate_actor", str);
    }

    void ApplyForce(Module *app, unsigned int id, float magnitude, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("apply_force", str);
    }

    void ApplyForce(Module *app, unsigned int id, float magnitude, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("apply_force", str);
    }

    void ApplyTorque(Module *app, unsigned int id, float magnitude, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("apply_torque", str);
    }

    void ApplyTorque(Module *app, unsigned int id, float magnitude, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        m.set_magnitude(magnitude);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("apply_torque", str);
    }

    void SetVelocity(Module *app, unsigned int id, std::string direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("set_velocity", str);
    }

    void SetVelocity(Module *app, unsigned int id, glm::vec3 direction)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);
        (*m.mutable_direction()) = Vec3(direction);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("set_velocity", str);
    }

    void Stop(Module *app, unsigned int id)
    {
        std::string str;

        ApplyForceMessage m;
        m.set_id(id);

        m.SerializeToString(&str);

        app->GetMessageManager()->Trigger("stop", str);
    }
}
