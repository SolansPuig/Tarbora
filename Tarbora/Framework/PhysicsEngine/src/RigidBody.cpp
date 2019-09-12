#include "../inc/RigidBody.hpp"

namespace Tarbora {
    void RigidBody::SetProperties(float friction, float density, float restitution)
    {
        m_Friction = friction;
        m_Density = density;
        m_Restitution = restitution;
        CalcVolume();
    }

    void RigidBody::ApplyImpulse(float newtons, const glm::vec3 &direction)
    {
        PhysicsEngine::ApplyImpulse(m_Body, newtons, direction);
    }

    void RigidBody::ApplyForce(float newtons, const glm::vec3 &direction)
    {
        PhysicsEngine::ApplyForce(m_Body, newtons, direction);
    }

    void RigidBody::ApplyTorque(float magnitude, const glm::vec3 &direction)
    {
        PhysicsEngine::ApplyTorque(m_Body, magnitude, direction);
    }

    void RigidBody::SetVelocity(const glm::vec3 &velocity)
    {
        PhysicsEngine::SetVelocity(m_Body, velocity);
    }

    void RigidBody::Stop()
    {
        PhysicsEngine::Stop(m_Body);
    }

    void RigidBody::RestrictRotation(float x, float y, float z)
    {
        PhysicsEngine::RestrictRotation(m_Body, x, y, z);
    }

    void RigidBody::SetLinearDamping(float damping)
    {
        m_Body->setDamping(damping, 0.0f);
    }

    std::shared_ptr<RayCastResult> RigidBody::RayCast(glm::vec3 origin, glm::vec3 direction, float distance)
    {
        ActorMotionState * motionState = static_cast<ActorMotionState*>(m_Body->getMotionState());
        glm::vec3 position = motionState->getPosition();
        glm::mat3 rotation = motionState->getRotation();
        glm::vec3 rayOrigin = origin + position;
        glm::vec3 rayDirection = glm::normalize(rotation * direction);
        glm::vec3 rayEnd = rayOrigin + rayDirection * distance;
        std::shared_ptr<RayCastResult> result = PhysicsEngine::RayCast(rayOrigin, rayEnd);
        return result;
    }


    SphereBody::SphereBody(float radius)
    {
        m_Radius = radius;
    }

    SphereBody::~SphereBody()
    {
        Unregister();
    }

    void SphereBody::Register(unsigned int id, glm::mat4 &transform)
    {
        m_Body = PhysicsEngine::AddSphere(id, m_Radius, m_Mass, m_Friction, m_Restitution, transform);
    }

    void SphereBody::Unregister()
    {
        PhysicsEngine::RemoveObject(m_Body);
    }

    void SphereBody::CalcVolume() {
        m_Volume = (4.f/3.f) * M_PI * m_Radius * m_Radius * m_Radius;
        CalcMass();
    }

    CapsuleBody::CapsuleBody(float radius, float height)
    {
        m_Radius = radius;
        m_Height = height;
    }

    CapsuleBody::~CapsuleBody()
    {
        Unregister();
    }

    void CapsuleBody::Register(unsigned int id, glm::mat4 &transform)
    {
        m_Body = PhysicsEngine::AddCapsule(id, m_Radius, m_Height, m_Mass, m_Friction, m_Restitution, transform);
    }

    void CapsuleBody::Unregister()
    {
        PhysicsEngine::RemoveObject(m_Body);
    }

    void CapsuleBody::CalcVolume() {
        m_Volume = ((4.f/3.f) * m_Radius + m_Height) * M_PI * m_Radius * m_Radius;
        CalcMass();
    }

    BoxBody::BoxBody(glm::vec3 &dimensions)
    {
        m_Dimensions = dimensions;
    }

    BoxBody::~BoxBody()
    {
        Unregister();
    }

    void BoxBody::Register(unsigned int id, glm::mat4 &transform)
    {
        m_Body = PhysicsEngine::AddBox(id, m_Dimensions, m_Mass, m_Friction, m_Restitution, transform);
    }

    void BoxBody::Unregister()
    {
        PhysicsEngine::RemoveObject(m_Body);
    }

    void BoxBody::CalcVolume() {
        m_Volume = m_Dimensions.x * m_Dimensions.y * m_Dimensions.z;
        CalcMass();
    }
}
