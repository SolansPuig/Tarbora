#include "PhysicsComponent.hpp"
#include "TransformComponent.hpp"
#include "../Messages/BasicMessages.hpp"

namespace Tarbora {
    PhysicsComponent::PhysicsComponent(System *s, const ActorId &id, const LuaTable &table) :
        Component(s, id, table)
    {
        std::string shape = table.get<std::string>("shape", "null");

        if (shape == "sphere")
        {
            height_ = table.get<float>("radius");
            body_ = std::shared_ptr<RigidBody>(new SphereBody(height_));
        }
        else if (shape == "capsule")
        {
            float radius = table.get<float>("radius");
            float height = table.get<float>("height");
            body_ = std::shared_ptr<RigidBody>(new CapsuleBody(radius, height));
            height_ = 2 * radius + height;
        }
        else if (shape == "box")
        {
            glm::vec3 size = table.get<glm::vec3>("size");
            body_ = std::shared_ptr<RigidBody>(new BoxBody(size));
            height_ = size.y;
        }
        else
        {
            LOG_ERR("PhysicsComponent: \"%s\" is not a valid character controller shape.", shape.c_str());
            error_ = true;
            return;
        }

        body_->setProperties(
            table.get<float>("friction", 0.f, true),
            table.get<float>("density", 1.f, true),
            table.get<float>("restitution", 0.f, true)
        );
    }

    void PhysicsComponent::setTransform(const glm::vec3 &position, const glm::quat &rotation)
    {
        if (enabled_)
            body_->setTransform(position, rotation);
    }

    void PhysicsComponent::setAngularFactor(const glm::vec3 &direction)
    {
        if (enabled_)
            body_->setAngularFactor(direction);
    }

    void PhysicsComponent::setLinearDamping(float value)
    {
        if (enabled_)
            body_->setLinearDamping(value);
    }

    void PhysicsComponent::setVelocity(const glm::vec3 &direction)
    {
        if (enabled_)
            body_->setVelocity(direction);
    }

    void PhysicsComponent::setAngularVelocity(const glm::vec3 &direction)
    {
        if (enabled_)
            body_->setAngularVelocity(direction);
    }

    void PhysicsComponent::applyForce(const glm::vec3 &direction)
    {
        if (enabled_)
            body_->applyForce(direction);
    }

    void PhysicsComponent::applyTorque(const glm::vec3 &direction)
    {
        if (enabled_)
            body_->applyTorque(direction);
    }

    void PhysicsComponent::stop()
    {
        if (enabled_)
            body_->stop();
    }

    std::shared_ptr<RayCastResult> PhysicsComponent::rayCast(const glm::vec3 &origin, const glm::quat &direction, float length)
    {
        if (enabled_)
            return body_->rayCast(origin, direction, length);
        else
            return std::shared_ptr<RayCastResult>();
    }

    void PhysicsComponent::onEnable()
    {
        if (transform_)
            transform_->setController(this);
    }

    void PhysicsComponent::onDisable()
    {
        if (transform_)
            transform_->setController(nullptr);
    }

    void PhysicsComponent::pick()
    {
        if (body_)
            body_->pick();
    }

    void PhysicsComponent::pick(const glm::vec3 &position)
    {
        if (body_)
            body_->pick(position);
    }

    void PhysicsComponent::movePicked(const glm::vec3 &position)
    {
        if (body_)
            body_->movePicked(position);
    }

    void PhysicsComponent::unpick()
    {
        if (body_)
            body_->unpick();
    }

    PhysicsSystem::PhysicsSystem(World *w) :
        SystemImpl<PhysicsComponent>(w)
    {
        PhysicsEngine::init();

        subscribe("apply_force", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(m.getId()));
            if (physics)
                physics->applyForce(m.getDirection());
        });

        subscribe("apply_torque", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(m.getId()));
            if (physics)
                physics->applyTorque(m.getDirection());
        });

        subscribe("set_velocity", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(m.getId()));
            if (physics)
                physics->setVelocity(m.getDirection());
        });

        subscribe("set_angular_velocity", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(m.getId()));
            if (physics)
                physics->setAngularVelocity(m.getDirection());
        });

        subscribe("stop", [this](const MessageSubject &subject, const MessageBody &body)
        {
            UNUSED(subject);
            Message::ApplyPhysics m(body);
            PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(m.getId()));
            if (physics)
                physics->stop();
        });
    }

    PhysicsSystem::~PhysicsSystem()
    {
        components_.clear();
        PhysicsEngine::close();
    }

    void PhysicsSystem::init(const ActorId &id)
    {
        PhysicsComponent *physics = static_cast<PhysicsComponent*>(get(id));
        if (physics && !physics->error())
        {
            physics->transform_ = static_cast<TransformComponent*>(getComponent(id, "transform"));
            if (physics->transform_)
            {
                physics->body_->registerActor(
                    physics->owner_,
                    physics->transform_->getPosition(),
                    physics->transform_->getRotation()
                );
            }

            physics->enable();
        }
    }

    void PhysicsSystem::update(float delta_time)
    {
        PhysicsEngine::update(delta_time);

        for (auto &component : components_)
        {
            PhysicsComponent *physics = &component.second;
            if (physics->enabled_)
            {
                if (physics->body_ && physics->transform_)
                {
                    if (physics->transform_->getPosition() != physics->body_->getPosition())
                        physics->transform_->setPosition(physics->body_->getPosition());

                    if (physics->transform_->getRotation() != physics->body_->getRotation())
                        physics->transform_->setRotation(physics->body_->getRotation());
                }
            }
        }
    }
}
