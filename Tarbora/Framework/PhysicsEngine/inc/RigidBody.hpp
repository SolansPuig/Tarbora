#pragma once
#include "PhysicsEngine.hpp"

namespace Tarbora {
    typedef enum { Sphere, Box } Shape;

    class RigidBody
    {
    public:
        RigidBody() {}
        ~RigidBody() {}

        virtual Shape GetType() = 0;

        void SetProperties(float friction, float density, float resitution);
        virtual void Register(unsigned int id, glm::mat4 &transform) = 0;
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

        void ApplyForce(float newtons, const glm::vec3 &direction);
        void ApplyTorque(float magnitude, const glm::vec3 &direction);
        void SetVelocity(const glm::vec3 &velocity);
        void Stop();

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
        virtual void Register(unsigned int id, glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        void SetRadius(float radius) { m_Radius = radius; CalcVolume(); }
        float GetRadius() { return m_Radius; }

    protected:
        float m_Radius;
    };

    class BoxBody : public RigidBody
    {
    public:
        BoxBody(glm::vec3 &dimensions);
        ~BoxBody();

        virtual Shape GetType() override { return Shape::Box; }
        virtual void Register(unsigned int id, glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        void SetDimensions(glm::vec3 &dimensions) { m_Dimensions = dimensions; CalcVolume(); }
        glm::vec3 &GetRadius() { return m_Dimensions; }

    protected:
        glm::vec3 m_Dimensions;
    };
}
