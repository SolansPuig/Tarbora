#include "ControllerComponent.hpp"
#include "TransformComponent.hpp"
#include "PhysicsComponent.hpp"
#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    ControllerComponent::ControllerComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        speed_ = table.get<float>("speed", 5.f);
        run_speed_ = table.get<float>("run_speed", 8.f);
        look_direction_ = glm::quat(glm::vec3(0.f, 0.f, 1.f));
        movement_ = glm::vec3(0.f);
        facing_ = glm::vec3(0.f);
        look_direction_ = glm::quat();
    }

    void ControllerComponent::setMovement(const glm::vec3 &direction)
    {
        if (enabled_)
        {
            movement_ = speed_ * direction;

            bool walking = speed_ != 0.f && glm::length(direction) != 0.f;
            if (animation_)
            {
                if (walking)
                    animation_->set("walk");
                else
                    animation_->set("idle");
            }
        }
    }

    void ControllerComponent::setLookDirection(const glm::vec3 &direction)
    {
        if (enabled_)
        {
            facing_.x = facing_.x - direction.y;
            facing_.y = facing_.y + direction.x;

            if (facing_.x >= 90.f)
                facing_.x = 90.f;
            else if (facing_.x <= -90.f)
                facing_.x = -90.f;

            glm::vec3 facing = glm::radians(facing_);

            trigger("move_node", Message::MoveNode(owner_, "head", glm::quat(glm::vec3(facing.x, 0.f, 0.f))));
            trigger("move_node", Message::MoveNode(owner_, "neck", glm::quat(glm::vec3(0.f, facing.y, 0.f))));

            look_direction_ = glm::quat(glm::vec3(facing.x, 0.f, -facing.y)) * glm::quat(glm::vec3(0.f, 0.f, 1.f));
        }
    }

    void ControllerComponent::checkGround()
    {
        // Find if the character is on the ground or falling.
        std::shared_ptr<RayCastResult> result = physics_->rayCast(
            glm::vec3(0.f),
            glm::quat(glm::vec3(0.f, 1.f, 0.f)),
            physics_->getHeight()/2.f
        );

        if (result)
        {
            if (!on_ground_)
            {
                // LOG_DEBUG("Just hit the ground");
            }
            physics_->setLinearDamping(0.9999999f);
            on_ground_ = true;
        }
        else
        {
            physics_->setLinearDamping(0.f);
            on_ground_ = false;
        }
    }

    void ControllerComponent::checkLookAt()
    {
        std::shared_ptr<RayCastResult> result = physics_->rayCast(
            glm::vec3(0.f, 0.f, 0.f),
            look_direction_,
            10.f
        );

        ActorId target = "";
        if (result)
        {
            target = result->hit_id;
            target_distance_ = result->distance;
        }

        if (target_ != target)
        {
            target_ = target;
            trigger("look_at", Message::LookAt(owner_, target_));
        }
    }

    ControllerSystem::ControllerSystem(World *w) :
        SystemImpl<ControllerComponent>(w)
    {
        subscribe("set_movement", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            ControllerComponent *controller = static_cast<ControllerComponent*>(get(m.getId()));
            if (controller)
                controller->setMovement(m.getDirection());
        });

        subscribe("look_direction", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::LookAt m(body);
            ControllerComponent *controller = static_cast<ControllerComponent*>(get(m.getId()));
            if (controller)
                controller->setLookDirection(m.getDirection());
        });
    }

    void ControllerSystem::init(const ActorId &id)
    {
        ControllerComponent *controller = static_cast<ControllerComponent*>(get(id));
        if (controller && !controller->error())
        {
            controller->transform_ = static_cast<TransformComponent*>(getComponent(id, "transform"));

            controller->physics_ = static_cast<PhysicsComponent*>(getComponent(id, "physics"));
            if (controller->physics_)
            {
                controller->physics_->setAngularFactor(glm::vec3(0.0f, 1.0f, 0.0f));
            }

            controller->animation_ = static_cast<AnimationComponent*>(getComponent(id, "animation"));
            if (controller->animation_)
            {
                controller->animation_->set("idle");
            }

            controller->enable();
        }
    }

    void ControllerSystem::update(float delta_time)
    {
        UNUSED(delta_time);
        for (auto &component : components_)
        {
            ControllerComponent *controller = &component.second;
            if (controller->enabled_)
            {
                controller->checkGround();
                controller->checkLookAt();
                controller->physics_->setVelocity(controller->movement_);
            }
        }
    }
}
