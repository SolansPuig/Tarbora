#include "ControllerComponent.hpp"
#include "PhysicsComponent.hpp"
#include "AnimationComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    ControllerComponent::ControllerComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_ShouldUpdate = false;
        m_Speed = table.Get<float>("speed", 5.f);
        m_RunSpeed = table.Get<float>("run_speed", 8.f);
        m_Movement = glm::vec3(0.f);
        m_Rotation = glm::vec3(0.f);
        m_LookRotation = glm::vec3(0.f);
    }

    void ControllerComponent::SetMovement(const glm::vec3 &direction)
    {
        if (m_Enabled)
        {
            m_Movement = m_Speed * direction;
            m_Walking = m_Speed != 0.f && glm::length(direction) != 0.f;
            if (m_Animation)
            {
                if (m_Walking)
                    m_Animation->SetAnimation("walk");
                else
                    m_Animation->SetAnimation("idle");
            }
        }
    }

    void ControllerComponent::SetLookDirection(const glm::vec3 &direction)
    {
        m_ShouldUpdate = true;

        m_LookRotation.x -= direction.y;
        m_Rotation.y = direction.x;

        if (m_LookRotation.x >= 70.f)
            m_LookRotation.x = 70.f;
        else if (m_LookRotation.x <= -70.f)
            m_LookRotation.x = -70.f;
    }

    void ControllerComponent::CheckGround()
    {
        // Find if the character is on the ground or falling.
        std::shared_ptr<RayCastResult> result = m_Physics->RayCast(
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, -1.f, 0.f),
            m_Physics->GetHeight()/2.f
        );

        if (result)
        {
            if (!m_OnGround)
            {
                // LOG_DEBUG("Just hit the ground");
            }
            m_Physics->SetLinearDamping(0.9999999f);
            m_OnGround = true;
        }
        else
        {
            m_Physics->SetLinearDamping(0.f);
            m_OnGround = false;
        }
    }

    ControllerSystem::ControllerSystem(World *w) :
        SystemImpl<ControllerComponent>(w)
    {
        Subscribe("set_movement", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            ControllerComponent *controller = static_cast<ControllerComponent*>(Get(m.GetId()));
            if (controller && controller->m_Enabled)
                controller->SetMovement(m.GetDirection());
        });

        Subscribe("look_direction", [this](MessageSubject subject, MessageBody *body)
        {
            Message::LookAt m(body);
            ControllerComponent *controller = static_cast<ControllerComponent*>(Get(m.GetId()));
            if (controller && controller->m_Enabled)
                controller->SetLookDirection(m.GetDirection());
        });
    }

    void ControllerSystem::Init(const ActorId &id)
    {
        ControllerComponent *controller = static_cast<ControllerComponent*>(Get(id));
        if (controller && !controller->Error())
        {
            controller->m_Physics = static_cast<PhysicsComponent*>(GetComponent(id, "physics"));
            if (controller->m_Physics)
            {
                controller->m_Physics->RestrictRotation(glm::vec3(0.0f, 1.0f, 0.0f));
            }

            controller->m_Animation = static_cast<AnimationComponent*>(GetComponent(id, "animation"));
            if (controller->m_Animation)
            {
                controller->m_Animation->SetAnimation("idle");
            }

            controller->Enable();
        }
    }

    void ControllerSystem::Update(float deltaTime)
    {
        for (auto &component : m_Components)
        {
            ControllerComponent *controller = &component.second;
            if (controller->m_Enabled)
            {
                controller->CheckGround();
                controller->m_Physics->SetVelocity(1.f, controller->m_Movement);
                controller->m_Physics->SetAngularVelocity(1.f, controller->m_Rotation);

                if (controller->m_ShouldUpdate)
                {
                    Trigger("move_node", Message::MoveNode(controller->m_Owner, "head", Rotation(controller->m_LookRotation)));
                    controller->m_ShouldUpdate = false;
                }
            }
        }
    }
}
