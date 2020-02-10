#include "ControllerComponent.hpp"
#include "PhysicsComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    ControllerComponent::ControllerComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        m_Speed = table.Get<float>("speed", 5.f);
        m_RunSpeed = table.Get<float>("run_speed", 8.f);
        m_Movement = glm::vec3(0.f);
        m_Rotation = glm::vec3(0.f);
        m_LookRotation = glm::vec3(0.f);
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
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();
            ControllerComponent *controller = static_cast<ControllerComponent*>(Get(m.id()));
            if (controller && controller->m_Enabled)
                controller->m_Movement = controller->m_Speed * Vec3toGLM(m.direction());
        });

        Subscribe("look_direction", [this](MessageSubject subject, MessageBody *body)
        {

            LookDirectionBody m = body->GetContent<LookDirectionBody>();
            ControllerComponent *controller = static_cast<ControllerComponent*>(Get(m.id()));
            if (controller && controller->m_Enabled)
            {
                glm::vec2 lookDirection = Vec2toGLM(m.direction());
                controller->m_Rotation.y = lookDirection.x;
                controller->m_LookRotation.x -= lookDirection.y;
                glm::vec3 headRotation(-lookDirection.y, 0.f, 0.f);
                if (controller->m_LookRotation.x >= 89.f) {
                    controller->m_LookRotation.x = 89.f;
                    headRotation.x = 0.0f;
                }
                else if (controller->m_LookRotation.x <= -89.f) {
                    controller->m_LookRotation.x = -89.f;
                    headRotation.x = 0.0f;
                }
                Trigger("move_node", MoveNode(m.id(), "head", glm::vec3(0.f), headRotation));
            }
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
            }
        }
    }
}
