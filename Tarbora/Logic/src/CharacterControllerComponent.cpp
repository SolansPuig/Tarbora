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

        m_StepHeight = 0.3;
        resource->Get(data, "stepHeight", &m_StepHeight);

        if (shape != "null")
        {
            if (shape == "sphere")
            {
                m_Body.reset(new SphereBody(resource->GetFloat(data, "radius") - m_StepHeight));
                m_Height = resource->GetFloat(data, "radius");
                ret = true;
            }
            else if (shape == "capsule")
            {
                float radius = resource->GetFloat(data, "radius");
                float height = resource->GetFloat(data, "height") - m_StepHeight;
                m_Body.reset(new CapsuleBody(radius, height));
                m_Height = 2 * radius + height;
                m_Width = 2 * radius;
                ret = true;
            }
            else if (shape == "box")
            {
                glm::vec3 dimensions = glm::vec3(
                    resource->GetFloatArray(data, "size", 0),
                    resource->GetFloatArray(data, "size", 1) - m_StepHeight,
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
            m_Body->RestrictRotation(0.0f, 0.0f, 0.0f);
        }

        Subscribe("apply_force", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
            {
                m_Body->ApplyImpulse(m.magnitude(), Vec3toGLM(m.direction()));
            }
        });

        Subscribe("apply_torque", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
            {
                m_Body->ApplyTorque(m.magnitude(), Vec3toGLM(m.direction()));
            }
        });

        Subscribe("set_velocity", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
                m_Movement = Vec3toGLM(m.direction());
        });

        Subscribe("stop", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
                m_Body->Stop();
        });
    }

    void CharacterControllerComponent::Update(float deltaTime)
    {
        ActorId id = m_Owner->GetId();

        m_Body->SetLinearDamping(0.9999999f);

        m_OnGround = false;
        float springForce = 0.f;
        glm::vec3 springDirection = glm::vec3(0.f, 1.f, 0.f);

        float halfHeight = m_Height/2.f;
        float halfWidth = m_Width/2.f;

        // Find if the character is on the ground or falling and adjust forces accordingly.
        float minDistance = 10;
        for (int i = 0; i < 4; i++)
        {
            float x = (i < 2 ? halfWidth : -halfWidth);
            float z = (i % 2 == 0 ? halfWidth : -halfWidth);
            std::shared_ptr<RayCastResult> result = m_Body->RayCast(glm::vec3(x, 0.f, z), glm::vec3(0.f, -1.f, 0.f), 10);

            if (result)
            {
                if (minDistance > result->m_Distance)
                {
                    minDistance = result->m_Distance;
                    springDirection = result->m_HitNormal;
                }
            }
        }

        if (minDistance <= halfHeight + 1.1 * m_StepHeight)
        {
            m_OnGround = true;
            springForce = halfHeight + m_StepHeight - minDistance;

            float slopeAngle = glm::degrees(acos(springDirection.y));
            if (slopeAngle > 10)
            {
                // Character is on a slope. Increase gravity to avoid jumping.
                LOG_DEBUG("On slope %f", slopeAngle);
                springForce *= 40.f;
            }
            else
            {
                springDirection = glm::vec3(0.f, 1.f, 0.f);
            }

            m_Body->ApplyForce(springForce, springDirection);
            // LOG_DEBUG("On ground");
        }
        else
        {
            m_Body->SetLinearDamping(0.01f);
            // LOG_DEBUG("Falling");
        }

        m_Body->SetVelocity(m_Movement - springDirection * glm::dot(m_Movement, springDirection));

        ActorMotionState *motionState = static_cast<ActorMotionState*>(m_Body->GetBody()->getMotionState());

        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));

        if (motionState)
        {
            if (transform)
            {
                if (transform->GetTransform() != motionState->m_Transform)
                {
                    transform->SetTransform(motionState->m_Transform);
                    Trigger("move_actor", MoveActor(id, motionState->getPosition(), motionState->getRotation()));
                }
            }
        }
    }
}
