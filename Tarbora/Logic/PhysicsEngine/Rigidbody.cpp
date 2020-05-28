/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#include "Rigidbody.hpp"
#include "PhysicsEngine.hpp"

namespace Tarbora {
  Rigidbody::Rigidbody(const ActorId &id)
  {
    user_pointer_ = id;
  }

  void Rigidbody::create(const glm::vec3 &position, const glm::quat &orientation)
  {
    mass = shape.volume * density;

    btVector3 inertia(0.f, 0.f, 0.f);
    if (mass > 0.f) shape.shape->calculateLocalInertia(mass, inertia);

    ActorMotionState *motion_state = new ActorMotionState(position, orientation);
    btRigidBody::btRigidBodyConstructionInfo info(
      mass, motion_state, shape.shape, inertia);
    info.m_restitution = restitution;
    info.m_friction = friction;

    body_ = std::shared_ptr<btRigidBody>(new btRigidBody(info));

    body_->setUserPointer(&user_pointer_);

    PhysicsEngine::registerRigidbody(this);

    enabled_ = true;
  }

  void Rigidbody::destroy()
  {
    PhysicsEngine::deleteRigidbody(this);
    enabled_ = false;
  }

  Rigidbody::~Rigidbody()
  {
    if (enabled_)
    {
      PhysicsEngine::deleteRigidbody(this);
    }

    if (body_)
    {
      delete body_->getMotionState();
      delete body_->getCollisionShape();
    }
  }

  void Rigidbody::setTransform(const glm::vec3 &position, const glm::quat &rotation)
  {
    btTransform transform(
      PhysicsEngine::toBullet(rotation),
      PhysicsEngine::toBullet(position)
    );
    body_->setWorldTransform(transform);
    body_->getMotionState()->setWorldTransform(transform);
    body_->activate();
  }

  void Rigidbody::setPosition(const glm::vec3 &position)
  {
    auto transform = body_->getWorldTransform();
    transform.setOrigin(PhysicsEngine::toBullet(position));
    body_->setWorldTransform(transform);
    body_->getMotionState()->setWorldTransform(transform);
    body_->activate();
  }

  void Rigidbody::setOrientation(const glm::quat &orientation)
  {
    auto transform = body_->getWorldTransform();
    transform.setRotation(PhysicsEngine::toBullet(orientation));
    body_->setWorldTransform(transform);
    body_->getMotionState()->setWorldTransform(transform);
    body_->activate();
  }

  void Rigidbody::move(const glm::vec3 &direction)
  {
    setPosition(direction + getPosition());
  }

  const glm::vec3& Rigidbody::getPosition()
  {
    return static_cast<ActorMotionState*>(body_->getMotionState())->getPosition();
  }

  const glm::quat& Rigidbody::getOrientation()
  {
    return static_cast<ActorMotionState*>(body_->getMotionState())->getOrientation();
  }

  glm::mat4 Rigidbody::getLocalTransform()
  {
    return PhysicsEngine::toGLM(body_->getWorldTransform().inverse());
  }

  glm::mat4 Rigidbody::getWorldTransform()
  {
    return PhysicsEngine::toGLM(body_->getWorldTransform());
  }

  void Rigidbody::applyForce(const glm::vec3 &vector)
  {
    body_->applyCentralForce(PhysicsEngine::toBullet(vector));
    body_->activate();
  }

  void Rigidbody::applyImpulse(const glm::vec3 &vector)
  {
    body_->applyCentralImpulse(PhysicsEngine::toBullet(getOrientation() * vector));
    body_->activate();
  }

  void Rigidbody::applyImpulse(const glm::vec3 &vector, const glm::vec3 &position)
  {
    body_->applyImpulse(
      PhysicsEngine::toBullet(vector),
      PhysicsEngine::toBullet(position) - body_->getCenterOfMassPosition()
    );
    body_->activate();
  }

  void Rigidbody::applyTorque(const glm::vec3 &vector)
  {
    body_->applyTorqueImpulse(PhysicsEngine::toBullet(vector));
    body_->activate();
  }

  void Rigidbody::setVelocity(const glm::vec3 &vector)
  {
    body_->setLinearVelocity(PhysicsEngine::toBullet(getOrientation() * vector));
    body_->activate();
  }

  void Rigidbody::setRotation(const glm::vec3 &vector)
  {
    body_->setAngularVelocity(PhysicsEngine::toBullet(vector));
    body_->activate();
  }

  void Rigidbody::setGravity(const glm::vec3 &vector)
  {
    body_->setGravity(PhysicsEngine::toBullet(vector));
    body_->activate();
  }

  RaycastResult Rigidbody::raycast(
    const glm::vec3 &origin,
    const glm::vec3 &direction,
    float distance
  )
  {
    const glm::vec3 ray_origin = origin + getPosition();
    const glm::vec3 ray_direction = getOrientation() * direction;
    const glm::vec3 ray_end = ray_origin + ray_direction * distance;
    return PhysicsEngine::raycast(ray_origin, ray_end);
  }

  void Rigidbody::setDamping(float linear, float angular)
  {
    body_->setDamping(linear, angular);
  }

  void Rigidbody::setAngularFactor(const glm::vec3 &axis)
  {
    body_->setAngularFactor(PhysicsEngine::toBullet(axis));
  }

  bool Rigidbody::isStatic()
  {
    return body_->isStaticOrKinematicObject();
  }

  void Rigidbody::alwaysActive(bool always)
  {
    if (always)
      body_->setActivationState(DISABLE_DEACTIVATION);
    else
      body_->forceActivationState(ACTIVE_TAG);
    body_->activate(true);
  }
}
