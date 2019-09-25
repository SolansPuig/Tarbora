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
        btVector3 const impulse(direction.x * newtons, direction.y * newtons, direction.z * newtons);
        m_Body->applyCentralImpulse(impulse);
        m_Body->activate();
    }

    void RigidBody::ApplyForce(float newtons, const glm::vec3 &direction)
    {
        btVector3 const force(direction.x * newtons, direction.y * newtons, direction.z * newtons);
        m_Body->applyCentralForce(force);
        m_Body->activate();
    }

    void RigidBody::ApplyTorque(float magnitude, const glm::vec3 &direction)
    {
        btVector3 const torque(direction.x * magnitude, direction.y * magnitude, direction.z * magnitude);
        m_Body->applyTorqueImpulse(torque);
        m_Body->activate();
    }

    void RigidBody::SetVelocity(const glm::vec3 &velocity)
    {
        glm::mat3 const rotation = static_cast<ActorMotionState*>(m_Body->getMotionState())->getRotation();
        glm::vec3 const globalVelocity = rotation * velocity;
        btVector3 const vel(globalVelocity.x, m_Body->getLinearVelocity().y(), globalVelocity.z);
        m_Body->setLinearVelocity(vel);
        m_Body->activate();
    }

    void RigidBody::SetAngularVelocity(const glm::vec3 &velocity)
    {
        btVector3 const vel(velocity.x, velocity.y, velocity.z);
        m_Body->setAngularVelocity(vel);
        m_Body->activate();
    }

    void RigidBody::Stop()
    {
        SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void RigidBody::RestrictRotation(const glm::vec3 &axis)
    {
        m_Body->setAngularFactor(btVector3(axis.x, axis.y, axis.z));
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
