#pragma once
#include "PhysicsEngine.hpp"
#include <memory>

namespace Tarbora {
    typedef enum { Sphere } Shape;

    class RigidBody
    {
    public:
        RigidBody() {}
        ~RigidBody() {}

        virtual Shape GetType() = 0;

        void SetProperties(float friction, float density, float resitution);
        virtual void Register(glm::mat4 &transform) = 0;
        virtual void Unregister() = 0;

        btRigidBody *Get() { return m_Body; }

        void SetFriction(float friction) { m_Friction = friction; }
        void SetDensity(float density) { m_Density = density; }
        void SetRestitution(float restitution) { m_Restitution = restitution; }
        float GetFriction() { return m_Friction; }
        float GetDensity() { return m_Density; }
        float GetRestitution() { return m_Restitution; }

        virtual void CalcVolume() = 0;
        void CalcMass() { m_Mass = m_Volume * m_Density; }
        float GetVolume() { return m_Volume; }
        float GetMass() { return m_Mass; }

        void Calc()
        {
            CalcVolume();
            CalcMass();
        }

    protected:
        float m_Friction, m_Density, m_Restitution, m_Volume, m_Mass;
        btRigidBody *m_Body;
    };

    typedef std::shared_ptr<RigidBody> RigidBodyPtr;

    class SphereBody : public RigidBody
    {
    public:
        SphereBody(float radius);
        ~SphereBody();

        virtual Shape GetType() override { return Shape::Sphere; }
        virtual void Register(glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        void SetRadius(float radius) { m_Radius = radius; CalcVolume(); }
        float GetRadius() { return m_Radius; }

    protected:
        float m_Radius;
    };
}
