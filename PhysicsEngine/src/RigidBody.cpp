#include "RigidBody.hpp"
#include "Logger.hpp"

namespace Tarbora {
    void RigidBody::SetProperties(float friction, float density, float restitution)
    {
        m_Friction = friction;
        m_Density = density;
        m_Restitution = restitution;
        Calc();
    }

    SphereBody::SphereBody(float radius)
    {
        m_Radius = radius;
    }

    SphereBody::~SphereBody()
    {
        Unregister();
    }

    void SphereBody::Register(glm::mat4 &transform)
    {
        m_Body = PhysicsEngine::AddSphere(m_Radius, m_Mass, m_Friction, m_Density, m_Restitution, transform);
    }

    void SphereBody::Unregister()
    {
        PhysicsEngine::RemoveObject(m_Body);
    }

    void SphereBody::CalcVolume() {
        m_Volume = (4.f/3.f) * M_PI * m_Radius * m_Radius * m_Radius;
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

    void BoxBody::Register(glm::mat4 &transform)
    {
        m_Body = PhysicsEngine::AddBox(m_Dimensions, m_Mass, m_Friction, m_Density, m_Restitution, transform);
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
