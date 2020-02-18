#include "RigidBody.hpp"

namespace Tarbora {
    void RigidBody::setProperties(float friction, float density, float restitution)
    {
        friction_ = friction;
        density_ = density;
        restitution_ = restitution;
        calcVolume();
    }

    void RigidBody::setTransform(const glm::vec3 &position, const glm::quat &rotation)
    {
        if (body_)
        {
            body_->setWorldTransform(btTransform(
                PhysicsEngine::toBullet(rotation),
                PhysicsEngine::toBullet(position)
            ));
        }
    }

    const glm::vec3& RigidBody::getPosition()
    {
        return static_cast<ActorMotionState*>(body_->getMotionState())
            ->getPosition();
    }

    const glm::quat& RigidBody::getRotation()
    {
        return static_cast<ActorMotionState*>(body_->getMotionState())
            ->getRotation();
    }

    void RigidBody::applyImpulse(const glm::vec3 &direction)
    {
        if (body_)
        {
            body_->applyCentralImpulse(PhysicsEngine::toBullet(direction));
            body_->activate();
        }
    }

    void RigidBody::applyForce(const glm::vec3 &direction)
    {
        if (body_)
        {
            body_->applyCentralForce(PhysicsEngine::toBullet(direction));
            body_->activate();
        }
    }

    void RigidBody::applyTorque(const glm::vec3 &direction)
    {
        if (body_)
        {
            body_->applyTorqueImpulse(PhysicsEngine::toBullet(direction));
            body_->activate();
        }
    }

    void RigidBody::setVelocity(const glm::vec3 &velocity)
    {
        if (body_)
        {
            const glm::quat rotation = getRotation();
            const glm::vec3 global_velocity = rotation * velocity;
            const btVector3 vel(global_velocity.x, body_->getLinearVelocity().y(), global_velocity.z);

            body_->setLinearVelocity(vel);
            body_->activate();
        }
    }

    void RigidBody::setAngularVelocity(const glm::vec3 &velocity)
    {
        if (body_)
        {
            body_->setAngularVelocity(PhysicsEngine::toBullet(velocity));
            body_->activate();
        }
    }

    void RigidBody::stop()
    {
        setVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void RigidBody::setAngularFactor(const glm::vec3 &axis)
    {
        if (body_)
            body_->setAngularFactor(PhysicsEngine::toBullet(axis));
    }

    void RigidBody::setLinearDamping(float damping)
    {
        if (body_)
            body_->setDamping(damping, 0.f);
    }

    std::shared_ptr<RayCastResult> RigidBody::rayCast(const glm::vec3 &origin, const glm::quat &direction, float distance)
    {
        if (body_)
        {
            const glm::vec3 position = getPosition();
            const glm::quat rotation = getRotation();
            const glm::vec3 ray_origin = origin + position;
            const glm::quat ray_direction = rotation * direction;
            const glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);
            const glm::vec3 ray_end = ray_origin + ray_direction * forward * distance;
            return PhysicsEngine::rayCast(ray_origin, ray_end);
        }
        return std::shared_ptr<RayCastResult>();
    }


    SphereBody::SphereBody(float radius)
    {
        radius_ = radius;
    }

    SphereBody::~SphereBody()
    {
        unregister();
    }

    void SphereBody::registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation)
    {
        body_ = PhysicsEngine::addSphere(id, radius_, mass_, friction_, restitution_, position, rotation);
    }

    void SphereBody::unregister()
    {
        if (body_)
            PhysicsEngine::removeObject(body_);
    }

    void SphereBody::calcVolume() {
        volume_ = (4.f/3.f) * M_PI * radius_ * radius_ * radius_;
        calcMass();
    }

    CapsuleBody::CapsuleBody(float radius, float height)
    {
        radius_ = radius;
        height_ = height;
    }

    CapsuleBody::~CapsuleBody()
    {
        unregister();
    }

    void CapsuleBody::registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation)
    {
        body_ = PhysicsEngine::addCapsule(id, radius_, height_, mass_, friction_, restitution_, position, rotation);
    }

    void CapsuleBody::unregister()
    {
        if (body_)
            PhysicsEngine::removeObject(body_);
    }

    void CapsuleBody::calcVolume() {
        volume_ = ((4.f/3.f) * radius_ + height_) * M_PI * radius_ * height_;
        calcMass();
    }

    BoxBody::BoxBody(const glm::vec3 &dimensions)
    {
        dimensions_ = dimensions;
    }

    BoxBody::~BoxBody()
    {
        unregister();
    }

    void BoxBody::registerActor(ActorId &id, const glm::vec3 &position, const glm::quat &rotation)
    {
        body_ = PhysicsEngine::addBox(id, dimensions_, mass_, friction_, restitution_, position, rotation);
    }

    void BoxBody::unregister()
    {
        if (body_)
            PhysicsEngine::removeObject(body_);
    }

    void BoxBody::calcVolume() {
        volume_ = dimensions_.x * dimensions_.y * dimensions_.z;
        calcMass();
    }
}
