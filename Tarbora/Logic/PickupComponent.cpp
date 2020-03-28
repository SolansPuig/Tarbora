#include "PickupComponent.hpp"
#include "TransformComponent.hpp"
#include "ControllerComponent.hpp"
#include "PhysicsComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    PickupComponent::PickupComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
    }

    void PickupComponent::pick(const ActorId &id, float distance, const glm::vec3 &position)
    {
        target_ = id;
        distance_ = std::max(1.f, distance);

        PhysicsComponent *target_physics = static_cast<PhysicsComponent*>(getComponent(target_, "physics"));
        if (target_physics)
            target_physics->pick(position);
    }

    void PickupComponent::unpick()
    {
        PhysicsComponent *target_physics = static_cast<PhysicsComponent*>(getComponent(target_, "physics"));
        if (target_physics)
            target_physics->unpick();

        target_ = "";
    }

    PickupSystem::PickupSystem(World *w) :
        SystemImpl<PickupComponent>(w)
    {
        subscribe("pick_object", [this](const MessageSubject&, const MessageBody &body)
        {
            Message::Actor m(body);
            PickupComponent *component = static_cast<PickupComponent*>(get(m.getId()));
            if (component)
            {
                LookAt look = component->controller_->getLookAt();
                component->pick(look.target, look.distance, look.position);
            }
        });

        subscribe("unpick_object", [this](const MessageSubject&, const MessageBody &body)
        {
            Message::Actor m(body);
            PickupComponent *component = static_cast<PickupComponent*>(get(m.getId()));
            if (component)
            {
                component->unpick();
            }
        });

        subscribe("pick_distance", [this](const MessageSubject&, const MessageBody &body)
        {
            Message::LookAt m(body);
            PickupComponent *component = static_cast<PickupComponent*>(get(m.getId()));
            if (component)
            {
                component->distance_ = std::max(1.f, component->distance_ + m.getDistance());
            }
        });
    }

    void PickupSystem::init(const ActorId &id)
    {
        PickupComponent *component = static_cast<PickupComponent*>(get(id));
        if (component && !component->error())
        {
            component->transform_ = static_cast<TransformComponent*>(getComponent(id, "transform"));
            component->controller_ = static_cast<ControllerComponent*>(getComponent(id, "controller"));
            if (component->controller_)
                component->enable();
            else
                LOG_ERR("Actor %s needs a valid controller to be allowed to have a pickup component", id.c_str());
        }
    }

    void PickupSystem::update(float)
    {
        for (auto &component : components_)
        {
            PickupComponent *comp = &component.second;
            if (comp->enabled_ && comp->target_ != "")
            {
                glm::vec3 direction = comp->transform_->getRotation() * comp->controller_->getLookDirection() * glm::vec3(0.f, 0.f, 1.f);
                glm::vec3 position = direction * comp->distance_ + comp->transform_->getPosition();
                PhysicsComponent *target_physics = static_cast<PhysicsComponent*>(getComponent(comp->target_, "physics"));
                if (target_physics)
                    target_physics->movePicked(position);
            }
        }
    }
}
