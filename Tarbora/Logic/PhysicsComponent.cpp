#include "PhysicsComponent.hpp"
#include "TransformComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    PhysicsComponent::PhysicsComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        std::string shape = table.Get<std::string>("shape", "null");

        if (shape == "sphere")
        {
            m_Height = table.Get<float>("radius");
            m_Body = std::shared_ptr<RigidBody>(new SphereBody(m_Height));
        }
        else if (shape == "capsule")
        {
            float radius = table.Get<float>("radius");
            float height = table.Get<float>("height");
            m_Body = std::shared_ptr<RigidBody>(new CapsuleBody(radius, height));
            m_Height = 2 * radius + height;
        }
        else if (shape == "box")
        {
            glm::vec3 size = table.Get<glm::vec3>("size");
            m_Body = std::shared_ptr<RigidBody>(new BoxBody(size));
            m_Height = size.y;
        }
        else
        {
            LOG_ERR("PhysicsComponent: \"%s\" is not a valid character controller shape.", shape.c_str());
            m_Error = true;
            return;
        }

        m_Body->SetProperties(
            table.Get<float>("friction", 0.f, true),
            table.Get<float>("density", 1.f, true),
            table.Get<float>("restitution", 0.f, true)
        );
    }

    void PhysicsComponent::UpdateTransform()
    {
        if (m_Enabled)
        {
            m_Body->SetTransform(m_Transform->GetTransform());
        }
    }

    void PhysicsComponent::RestrictRotation(const glm::vec3 &rotation)
    {
        if (m_Enabled)
            m_Body->RestrictRotation(glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void PhysicsComponent::SetLinearDamping(float value)
    {
        if (m_Enabled)
            m_Body->SetLinearDamping(value);
    }

    void PhysicsComponent::SetVelocity(float magnitude, const glm::vec3 &direction)
    {
        if (m_Enabled)
            m_Body->SetVelocity(magnitude * direction);
    }

    void PhysicsComponent::SetAngularVelocity(float magnitude, const glm::vec3 &direction)
    {
        if (m_Enabled)
            m_Body->SetAngularVelocity(magnitude * direction);
    }

    void PhysicsComponent::ApplyForce(float magnitude, const glm::vec3 &direction)
    {
        if (m_Enabled)
            m_Body->ApplyForce(magnitude, direction);
    }

    void PhysicsComponent::ApplyTorque(float magnitude, const glm::vec3 &direction)
    {
        if (m_Enabled)
            m_Body->ApplyTorque(magnitude, direction);
    }

    void PhysicsComponent::Stop()
    {
        if (m_Enabled)
            m_Body->Stop();
    }

    std::shared_ptr<RayCastResult> PhysicsComponent::RayCast(const glm::vec3 &origin, const glm::vec3 &direction, float length)
    {
        if (m_Enabled)
            return m_Body->RayCast(origin, direction, length);
        else
            return std::shared_ptr<RayCastResult>();
    }

    void PhysicsComponent::OnEnable()
    {
        if (m_Transform)
            m_Transform->SetController(this);
    }

    void PhysicsComponent::OnDisable()
    {
        if (m_Transform)
            m_Transform->SetController(nullptr);
    }

    PhysicsSystem::PhysicsSystem(World *w) :
        SystemImpl<PhysicsComponent>(w)
    {
        PhysicsEngine::Init();

        Subscribe("apply_force", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(Get(m.GetId()));
            if (physics && physics->m_Enabled)
                physics->ApplyForce(m.GetMagnitude(), m.GetDirection());
        });

        Subscribe("apply_torque", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(Get(m.GetId()));
            if (physics && physics->m_Enabled)
                physics->ApplyTorque(m.GetMagnitude(), m.GetDirection());
        });

        Subscribe("set_velocity", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(Get(m.GetId()));
            if (physics && physics->m_Enabled)
                physics->SetVelocity(m.GetMagnitude(), m.GetDirection());
        });

        Subscribe("stop", [this](MessageSubject subject, MessageBody *body)
        {
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(Get(m.GetId()));
            if (physics && physics->m_Enabled)
                physics->Stop();
        });
    }

    PhysicsSystem::~PhysicsSystem()
    {
        m_Components.clear();
        PhysicsEngine::Close();
    }

    void PhysicsSystem::Init(const ActorId &id)
    {
        PhysicsComponent *physics = static_cast<PhysicsComponent*>(Get(id));
        if (physics && !physics->Error())
        {
            physics->m_Transform = static_cast<TransformComponent*>(GetComponent(id, "transform"));
            if (physics->m_Transform)
            {
                glm::mat4 transform = physics->m_Transform->GetTransform();
                physics->m_Body->Register(physics->m_Owner, transform);
            }

            physics->m_MotionState = static_cast<ActorMotionState*>(physics->m_Body->GetBody()->getMotionState());

            physics->Enable();
        }
    }

    void PhysicsSystem::Update(float deltaTime)
    {
        PhysicsEngine::Update(deltaTime);

        for (auto &component : m_Components)
        {
            PhysicsComponent *physics = &component.second;
            if (physics->m_Enabled)
            {
                if (physics->m_MotionState && physics->m_Transform)
                {
                    if (physics->m_Transform->GetTransform() != physics->m_MotionState->m_Transform)
                    {
                        physics->m_Transform->SetTransform(physics->m_MotionState->m_Transform);
                    }
                }
            }
        }
    }
}
