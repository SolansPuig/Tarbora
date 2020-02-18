#include "TransformComponent.hpp"
#include "PhysicsComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    TransformComponent::TransformComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        changed_ = false;
        controller_ = nullptr;

        setOrigin(table.get<glm::vec3>("origin", true));
        setPosition(table.get<glm::vec3>("position"));
        setRotation(table.get<glm::vec3>("rotation"));
    }

    void TransformComponent::setPosition(const glm::vec3 &position)
    {
        changed_ = true;
        position_ = position;
    }

    void TransformComponent::setRotation(const glm::vec3 &rotation)
    {
        setRotation(glm::quat(glm::radians(rotation)));
    }

    void TransformComponent::setRotation(const glm::quat &rotation)
    {
        changed_ = true;
        rotation_ = rotation;
    }

    void TransformComponent::setOrigin(const glm::vec3 &origin)
    {
        changed_ = true;
        origin_ = origin;
    }

    void TransformComponent::move(const glm::vec3 &position)
    {
        changed_ = true;
        position_ += position;
    }

    void TransformComponent::rotate(const glm::vec3 &rotation)
    {
        rotate(glm::quat(rotation));
    }

    void TransformComponent::rotate(const glm::quat &rotation)
    {
        changed_ = true;
        rotation_ *= rotation;
    }

    TransformSystem::TransformSystem(World *w) :
        SystemImpl<TransformComponent>(w)
    {
        subscribe("set_position", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(get(m.getId()));
            if (transform)
                transform->setPosition(m.getDirection());
        });

        subscribe("move", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(get(m.getId()));
            if (transform)
                transform->move(m.getDirection());
        });

        subscribe("set_rotation", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(get(m.getId()));
            if (transform)
                transform->setRotation(m.getDirection());
        });

        subscribe("rotate", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            TransformComponent *transform = static_cast<TransformComponent*>(get(m.getId()));
            if (transform)
                transform->rotate(m.getDirection());
        });
    }

    void TransformSystem::update(float delta_time)
    {
        UNUSED(delta_time);
        for (auto &component : components_)
        {
            TransformComponent *transform = &component.second;
            if (transform->enabled_ && transform->changed_)
            {
                trigger("move_actor", Message::MoveActor(
                    transform->owner_,
                    transform->getPosition(),
                    transform->getRotation()
                ));
                transform->changed_ = false;
            }
        }
    }
}
