#pragma once
#include "PhysicsEngine.hpp"

namespace Tarbora {
    typedef enum { Sphere, Capsule, Box } Shape;

    //! An abstract physics rigid body
    /*!
        \see PhysicsEngine
    */
    class RigidBody
    {
    public:
        RigidBody() {}
        ~RigidBody() {}

        //! Register the rigid body to the Physics Engine.
        /*!
            \param id The id of the actor that owns that rigid body.
            \param transform The transform matrix of that actor.
        */
        virtual void Register(unsigned int id, glm::mat4 &transform) = 0;

        //! Remove the rigid body from the Physics Engine.
        virtual void Unregister() = 0;

        //! Get the Bullet3's rigid body.
        btRigidBody *GetBody() { return m_Body; }

        //! Set all the properties that define a physics object.
        /*!
            \param friction See \ref SetFriction
            \param density See \ref SetDensity
            \param resitution See \ref SetRestitution
        */
        void SetProperties(float friction, float density, float resitution);

        //! Set the friction property.
        /*!
            This is a per object coeficient. Usually from 0 (like ice) to 1 (like rubber), but can be more.
        */
        void SetFriction(float friction) { m_Friction = friction; }

        //! Set the density property.
        /*!
            Used to calculate the mass ("weight") of the object, based on its volume ("size and shape").
        */
        void SetDensity(float density) { m_Density = density; }

        //! Set the resitution property.
        /*!
            This is a per object coeficient that determines the "bounciness" of the object. Usually from 0 (like play dough) to 1 (like a bouncing ball), but can be more.
        */
        void SetRestitution(float restitution) { m_Restitution = restitution; }

        //! Get the friction property.
        float GetFriction() { return m_Friction; }

        //! Get the density property.
        float GetDensity() { return m_Density; }

        //! Get the resitution property.
        float GetRestitution() { return m_Restitution; }

        //! Get the volume of the physics object.
        /*!
            This may be a bit different (usually smaller) than the size of the rendered model.
        */
        float GetVolume() { return m_Volume; }

        //! Get the mass of the object.
        float GetMass() { return m_Mass; }

        //! Apply an impulse on the center of the object.
        /*!
            \param newtons The strenght of that impulse.
            \param direction A normalized vector representing the direction of the impulse.
        */
        void ApplyImpulse(float newtons, const glm::vec3 &direction);

        //! Apply a force on the center of the object.
        /*!
            \param newtons The strenght of that force.
            \param direction A normalized vector representing the direction of the force.
        */
        void ApplyForce(float newtons, const glm::vec3 &direction);

        //! Apply a torque to the object. A force that will make the object rotate.
        /*!
            \param magnitude The strenght of that force.
            \param direction A normalized vector representing the direction of the force.
        */
        void ApplyTorque(float magnitude, const glm::vec3 &direction);

        //! Set a constant velocity to the object.
        /*!
            \param velocity A vector representing the speed in each of the axis (x, y and z).
        */
        void SetVelocity(const glm::vec3 &velocity);

        //! Set the object velocity to zero.
        void Stop();

        //! Restrict the rotation of the body in each axis.
        /*!
            \param x The multiplier to the rotation in the x axis. Set to 0 to forbid the rotation in x, set to 1 to leave it untouched.
            \param y The multiplier to the rotation in the y axis. Set to 0 to forbid the rotation in y, set to 1 to leave it untouched.
            \param z The multiplier to the rotation in the z axis. Set to 0 to forbid the rotation in z, set to 1 to leave it untouched.
        */
        void RestrictRotation(float x, float y, float z);

        //! Set the linear damping of the body.
        /*!
            \param damping The ammount of damping.
        */
        void SetLinearDamping(float damping);

        //! Perform a Raycast from the body.
        /*!
            \param origin The relative position from the center of the body from where the raycast is shoot.
            \param direction The relative direction of the raycast.
            \param distance The maximum distance of the raycast.
            \returns See \ref RayCastResult.
        */
        std::shared_ptr<RayCastResult> RayCast(glm::vec3 origin, glm::vec3 direction, float distance);

    protected:
        //! Calculate the volume when the shape or the size changes. Automatically called by the functions that change those.
        virtual void CalcVolume() = 0;

        //! Calculate the mass when the volume changes. Automatically called by CalcVolume.
        void CalcMass() { m_Mass = m_Volume * m_Density; }

        float m_Friction, m_Density, m_Restitution, m_Volume, m_Mass;
        btRigidBody *m_Body;
    };

    typedef std::shared_ptr<RigidBody> RigidBodyPtr;

    //! A physics rigid body representing an sphere.
    /*!
        \see PhysicsEngine
        \see RigidBody
        \see BoxBody
    */
    class SphereBody : public RigidBody
    {
    public:
        //! Creates an SphereBody.
        /*!
            \param radius The radius of that sphere, used to calculate the volume and the mass.
        */
        SphereBody(float radius);

        //! Destroys and unregisters this body.
        ~SphereBody();

        virtual void Register(unsigned int id, glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        //! Set the radius of that sphere.
        /*!
            \param radius The radius of that sphere, used to calculate the volume and the mass.
        */
        void SetRadius(float radius) { m_Radius = radius; CalcVolume(); }

        //! Get the radius of that sphere.
        float GetRadius() { return m_Radius; }

    private:
        float m_Radius;
    };

    //! A physics rigid body representing an capsule shape.
    /*!
        \see PhysicsEngine
        \see RigidBody
        \see SphereBody
    */
    class CapsuleBody : public RigidBody
    {
    public:
        //! Creates an CapsuleBody.
        /*!
            \param radius The radius of that capsule, used to calculate the volume and the mass.
            \param height The height of that capsule, used to calculate the volume and the mass.
        */
        CapsuleBody(float radius, float height);

        //! Destroys and unregisters this body.
        ~CapsuleBody();

        virtual void Register(unsigned int id, glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        //! Set the radius of that capsule.
        /*!
            \param radius The radius of that capsule, used to calculate the volume and the mass.
        */
        void SetRadius(float radius) { m_Radius = radius; CalcVolume(); }

        //! Get the radius of that sphere.
        float GetRadius() { return m_Radius; }

        //! Set the height of that capsule.
        /*!
            \param height The height of that capsule, used to calculate the volume and the mass.
        */
        void SetHeight(float height) { m_Height = height; CalcVolume(); }

        //! Get the height of that sphere.
        float GetHeight() { return m_Height; }

    private:
        float m_Radius;
        float m_Height;
    };

    //! A physics rigid body representing a cube.
    /*!
        \see PhysicsEngine
        \see RigidBody
        \see SphereBody
    */
    class BoxBody : public RigidBody
    {
    public:
        //! Creates a BoxBody.
        /*!
            \param dimensions A vector representing the dimensions of that box, used to calculate the volume and the mass.
        */
        BoxBody(glm::vec3 &dimensions);

        //! Destroys and unregisters this body.
        ~BoxBody();

        virtual void Register(unsigned int id, glm::mat4 &transform) override;
        virtual void Unregister() override;
        virtual void CalcVolume() override;

        //! Set the dimensions of that box, used to calculate the volume and the mass.
        /*!
            \param dimensions A vector representing the dimensions of that box.
        */
        void SetDimensions(glm::vec3 &dimensions) { m_Dimensions = dimensions; CalcVolume(); }

        //! Get the dimensions of that box.
        glm::vec3 &GetDimensions() { return m_Dimensions; }

    private:
        glm::vec3 m_Dimensions;
    };
}
