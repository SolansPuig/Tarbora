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
            \param position The initial position of that actor.
            \param rotation The initial rotation of that actor.
        */
        virtual void registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation ) = 0;

        //! Remove the rigid body from the Physics Engine.
        virtual void unregister() = 0;

        //! Set all the properties that define a physics object.
        /*!
            \param friction See \ref SetFriction
            \param density See \ref SetDensity
            \param resitution See \ref SetRestitution
        */
        void setProperties(float friction, float density, float resitution);

        //! Set the friction property.
        /*!
            This is a per object coeficient. Usually from 0 (like ice) to 1 (like rubber), but can be more.
        */
        void setFriction(float friction) { friction_ = friction; }

        //! Set the density property.
        /*!
            Used to calculate the mass ("weight") of the object, based on its volume ("size and shape").
        */
        void setDensity(float density) { density_ = density; }

        //! Set the resitution property.
        /*!
            This is a per object coeficient that determines the "bounciness" of the object. Usually from 0 (like play dough) to 1 (like a bouncing ball), but can be more.
        */
        void setRestitution(float restitution) { restitution_ = restitution; }

        //! Get the friction property.
        float getFriction() { return friction_; }

        //! Get the density property.
        float getDensity() { return density_; }

        //! Get the resitution property.
        float getRestitution() { return restitution_; }

        //! Get the volume of the physics object.
        /*!
            This may be a bit different (usually smaller) than the size of the rendered model.
        */
        float getVolume() { return volume_; }

        //! Get the mass of the object.
        float getMass() { return mass_; }

        //! Change the position and rotation of the object.
        void setTransform(const glm::vec3 &position, const glm::quat &rotation);

        const glm::vec3& getPosition();

        const glm::quat& getRotation();

        //! Apply an impulse on the center of the object.
        /*!
            \param direction A vector representing the direction of the impulse.
        */
        void applyImpulse(const glm::vec3 &direction);

        //! Apply a force on the center of the object.
        /*!
            \param direction A vector representing the direction of the force.
        */
        void applyForce(const glm::vec3 &direction);

        //! Apply a torque to the object. A force that will make the object rotate.
        /*!
            \param direction A vector representing the direction of the force.
        */
        void applyTorque(const glm::vec3 &direction);

        //! Set a constant velocity to the object.
        /*!
            \param velocity A vector representing the speed in each of the axis (x, y and z).
        */
        void setVelocity(const glm::vec3 &velocity);

        //! Set a constant angular velocity to the object.
        /*!
            \param velocity A vector representing the rotation speed in each of the roation axis (x, y and z).
        */
        void setAngularVelocity(const glm::vec3 &velocity);

        //! Set the object's velocity to zero.
        void stop();

        //! Set the angular factor of the body.
        /*!
            \param direction The multiplier to the rotation in each of the axis. Set to 0 to forbid the rotation in that axis, set to 1 to leave it untouched.
        */
        void setAngularFactor(const glm::vec3 &direction);

        //! Set the linear damping of the body. This will slow down the movement of the body.
        /*!
            \param damping The multiplier to the movement in each of the axis. Set to 0 to forbid the movement in that axis, set to 1 to leave it untouched.
        */
        void setLinearDamping(float damping);

        //! Perform a Raycast from the body.
        /*!
            \param origin The relative position from the center of the body from where the raycast is shoot.
            \param direction The relative direction of the raycast, a quaternion.
            \param distance The maximum reach of the raycast.
            \returns See \ref RayCastResult.
        */
        std::shared_ptr<RayCastResult> rayCast(const glm::vec3 &origin, const glm::quat &direction, float distance);

    protected:
        //! Calculate the volume when the shape or the size changes. Automatically called by the functions that change those.
        virtual void calcVolume() = 0;

        //! Calculate the mass when the volume changes. Automatically called by CalcVolume.
        void calcMass() { mass_ = volume_ * density_; }

        float friction_;
        float density_;
        float restitution_;
        float volume_;
        float mass_;
        btRigidBody *body_;
    };

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

        virtual void registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation) override;
        virtual void unregister() override;
        virtual void calcVolume() override;

        //! Set the radius of that sphere.
        /*!
            \param radius The radius of that sphere, used to calculate the volume and the mass.
        */
        void setRadius(float radius) { radius_ = radius; calcVolume(); }

        //! Get the radius of that sphere.
        float getRadius() { return radius_; }

    private:
        float radius_;
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

        virtual void registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation) override;
        virtual void unregister() override;
        virtual void calcVolume() override;

        //! Set the radius of that capsule.
        /*!
            \param radius The radius of that capsule, used to calculate the volume and the mass.
        */
        void setRadius(float radius) { radius_ = radius; calcVolume(); }

        //! Get the radius of that sphere.
        float getRadius() { return radius_; }

        //! Set the height of that capsule.
        /*!
            \param height The height of that capsule, used to calculate the volume and the mass.
        */
        void setHeight(float height) { height_ = height; calcVolume(); }

        //! Get the height of that sphere.
        float getHeight() { return height_; }

    private:
        float radius_;
        float height_;
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
            \param dimensions The dimensions of that box, used to calculate the volume and the mass.
        */
        BoxBody(const glm::vec3 &dimensions);

        //! Destroys and unregisters this body.
        ~BoxBody();

        virtual void registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation) override;
        virtual void unregister() override;
        virtual void calcVolume() override;

        //! Set the dimensions of the box, used to calculate the volume and the mass.
        /*!
            \param dimensions The dimensions of that box.
        */
        void setDimensions(glm::vec3 &dimensions) { dimensions_ = dimensions; calcVolume(); }

        //! Get the dimensions of that box.
        const glm::vec3& getDimensions() { return dimensions_; }

    private:
        glm::vec3 dimensions_;
    };
}
