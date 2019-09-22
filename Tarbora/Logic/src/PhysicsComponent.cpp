#include "../inc/PhysicsComponent.hpp"
#include "../inc/TransformComponent.hpp"
#include "../inc/World.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
    bool PhysicsComponent::Init(JsonPtr resource, raw_json data)
    {
        bool ret = false;
        std::string shape = "null";
        resource->Get(data, "shape", &shape);
        if (shape != "null")
        {
            if (shape == "sphere")
            {
                m_Body.reset(new SphereBody(resource->GetFloat(data, "radius")));
                ret = true;
            }
            else if (shape == "capsule")
            {
                m_Body.reset(new CapsuleBody(resource->GetFloat(data, "radius"), resource->GetFloat(data, "height")));
                ret = true;
            }
            else if (shape == "box")
            {
                glm::vec3 dimensions = glm::vec3(
                    resource->GetFloatArray(data, "size", 0),
                    resource->GetFloatArray(data, "size", 1),
                    resource->GetFloatArray(data, "size", 2)
                );
                m_Body.reset(new BoxBody(dimensions));
                ret = true;
            }

            if (ret)
            {
                float friction = 0.9;
                float density = 1;
                float restitution = 0.2;
                resource->Get(data, "friction", &friction, {true});
                resource->Get(data, "density", &density, {true});
                resource->Get(data, "restitution", &restitution, {true});
                m_Body->SetProperties(friction, density, restitution);
            }
            else
            {
                LOG_ERR("PhysicsComponent: \"%s\" is not a valid physics shape.", shape.c_str());
            }
        }
        return ret;
    }

    void PhysicsComponent::AfterInit()
    {
        std::shared_ptr<TransformComponent> transform = std::static_pointer_cast<TransformComponent>(m_Owner->GetComponent(TransformId));
        if (transform)
        {
            glm::mat4 transposed = transform->GetTransform();
            m_Body->Register(m_Owner->GetId(), transposed);
        }

        Subscribe("apply_force", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
            {
                m_Body->ApplyForce(m.magnitude(), Vec3toGLM(m.direction()));
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
                m_Body->SetVelocity(Vec3toGLM(m.direction()));
        });

        Subscribe("stop", [this](MessageSubject subject, MessageBody *body)
        {
            ApplyPhysicsBody m = body->GetContent<ApplyPhysicsBody>();

            if (m.id() == m_Owner->GetId())
                m_Body->Stop();
        });
    }

    void PhysicsComponent::Update(float deltaTime)
    {
        (void)(deltaTime);
        ActorId id = m_Owner->GetId();
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
