#include "../inc/CharacterControllerComponent.hpp"
#include "../inc/TransformComponent.hpp"
#include "../inc/World.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    bool CharacterControllerComponent::Init(JsonPtr resource, raw_json data)
    {
        bool ret = false;
        std::string shape = "null";
        resource->Get(data, "shape", &shape);

        m_Speed = 5.0f;
        m_RunSpeed = 8.0f;
        resource->Get(data, "speed", &m_Speed);
        resource->Get(data, "run_speed", &m_RunSpeed);

        m_LookRotation = glm::vec3(0.f, 0.f, 0.f);

        if (shape != "null")
        {
            if (shape == "sphere")
            {
                m_Body.reset(new SphereBody(resource->GetFloat(data, "radius")));
                m_Height = resource->GetFloat(data, "radius");
                ret = true;
            }
            else if (shape == "capsule")
            {
                float radius = resource->GetFloat(data, "radius");
                float height = resource->GetFloat(data, "height");
                m_Body.reset(new CapsuleBody(radius, height));
                m_Height = 2 * radius + height;
                m_Width = 2 * radius;
                ret = true;
            }
            else if (shape == "box")
            {
                glm::vec3 dimensions = glm::vec3(
                    resource->GetFloatArray(data, "size", 0),
                    resource->GetFloatArray(data, "size", 1) ,
                    resource->GetFloatArray(data, "size", 2)
                );
                m_Body.reset(new BoxBody(dimensions));
                ret = true;
            }

            if (ret)
            {
                float density = 1;
                resource->Get(data, "density", &density, {true});
                m_Body->SetProperties(0.0f, density, 0.0f);
            }
            else
            {
                LOG_ERR("CharacterControllerComponent: \"%s\" is not a valid character controller shape.", shape.c_str());
            }
        }
        return ret;
    }

    void CharacterControllerComponent::AfterInit()
    {
        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            glm::mat4 transposed = transform->GetTransform();
            m_Body->Register(m_Owner->GetId(), transposed);
            m_Body->RestrictRotation(glm::vec3(0.0f, 1.0f, 0.0f));
        }

        Subscribe("set_movement", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
            {
                m_Movement = m_Speed * Vec3toGLM(m.direction());
            }

        });

        Subscribe("look_direction", [this](MessageSubject subject, MessageBody *body)
        {
            LookDirectionBody m = body->GetContent<LookDirectionBody>();

            if (m.id() == m_Owner->GetId())
            {
                glm::vec2 lookDirection = Vec2toGLM(m.direction());
                m_Rotation.y = lookDirection.x;
                m_LookRotation.x -= lookDirection.y;
                if (m_LookRotation.x >= 89.f) m_LookRotation.x = 89.f;
                else if (m_LookRotation.x <= -89.f) m_LookRotation.x = -89.f;
                Trigger("move_node", MoveNode(m_Owner->GetId(), "head", glm::vec3(0.f, 0.f, 0.f), m_LookRotation));
            }
        });
    }

    void CharacterControllerComponent::CheckLineOfSight()
    {

    }

    void CharacterControllerComponent::CheckGround()
    {
        // Find if the character is on the ground or falling.
        std::shared_ptr<RayCastResult> result = m_Body->RayCast(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, -1.f, 0.f), m_Height/2.f);
        if (result)
        {
            if (!m_OnGround)
            {
                // LOG_DEBUG("Just hit the ground");
            }
            m_Body->SetLinearDamping(0.9999999f);
            m_OnGround = true;
        }
        else
        {
            m_Body->SetLinearDamping(0.f);
            m_OnGround = false;
        }
    }

    void CharacterControllerComponent::UpdateTransformAndView()
    {
        ActorMotionState *motionState = static_cast<ActorMotionState*>(m_Body->GetBody()->getMotionState());

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));

        if (motionState)
        {
            if (transform)
            {
                if (transform->GetTransform() != motionState->m_Transform)
                {
                    transform->SetTransform(motionState->m_Transform);
                    Trigger("move_actor", MoveActor(m_Owner->GetId(), motionState->getPosition(), motionState->getRotation()));
                }
            }
        }
    }

    void CharacterControllerComponent::Update(float deltaTime)
    {
        CheckGround();
        m_Body->SetVelocity(m_Movement);
        m_Body->SetAngularVelocity(m_Rotation);
        UpdateTransformAndView();
    }
}
