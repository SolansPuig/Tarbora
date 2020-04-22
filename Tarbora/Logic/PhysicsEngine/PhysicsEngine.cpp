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

#include "PhysicsEngine.hpp"

namespace Tarbora {
  bool PhysicsEngine::init()
  {
    // Configuration of Bullet3
    collision_configuration_ = new btDefaultCollisionConfiguration();
    dispatcher_ = new btCollisionDispatcher(collision_configuration_);
    broadphase_ = new btDbvtBroadphase();
    solver_ = new btSequentialImpulseConstraintSolver();
    dynamics_world_ = new btDiscreteDynamicsWorld(
      dispatcher_, broadphase_, solver_, collision_configuration_);

    if (!collision_configuration_ || !dispatcher_ || !broadphase_
        || !solver_ || !dynamics_world_)
    {
      LOG_ERR("PhysicsEngine: Initialization failed.");
      return false;
    }

    dynamics_world_->setInternalTickCallback(internalTickCallback);
    dynamics_world_->setGravity(btVector3(0, -9.81f, 0));

    return true;
  }

  void PhysicsEngine::close()
  {
    // Delete all registered objects.
    for (int i = dynamics_world_->getNumCollisionObjects()-1; i >= 0; i--)
    {
      btCollisionObject *obj = dynamics_world_->getCollisionObjectArray()[i];
      deleteObject(obj);
    }

    // Delete all Bullet3 variables.
    delete dynamics_world_;
    delete solver_;
    delete broadphase_;
    delete dispatcher_;
    delete collision_configuration_;
  }

  void PhysicsEngine::update(float delta_time)
  {
    dynamics_world_->stepSimulation(delta_time, 0);
  }

  void PhysicsEngine::registerRigidbody(Rigidbody *body)
  {
    btRigidBody *bt_body = body->body_.get();
    dynamics_world_->addRigidBody(bt_body);
  }

  void PhysicsEngine::deleteRigidbody(Rigidbody *body)
  {
    btRigidBody *bt_body = body->body_.get();
    deleteObject(bt_body);
  }

  void PhysicsEngine::deleteObject(btCollisionObject *object)
  {
    dynamics_world_->removeCollisionObject(object);

    // Remove all the collision pairs
    for (auto itr = previous_collisions_.begin(); itr != previous_collisions_.end();)
    {
      auto next = itr;
      next++;

      if (itr->first == object || itr->second == object)
      {
        previous_collisions_.erase(itr);
      }

      itr = next;
    }
  }

  void PhysicsEngine::registerConstraint(Constraint *constraint)
  {
    dynamics_world_->addConstraint(constraint->getConstraint().get(), true);
  }

  void PhysicsEngine::deleteConstraint(Constraint *constraint)
  {
    dynamics_world_->removeConstraint(constraint->getConstraint().get());
  }

  RaycastResult PhysicsEngine::raycast(const glm::vec3 &origin, const glm::vec3 &end)
  {
    RaycastResult result;

    btVector3 ray_origin = toBullet(origin);
    btVector3 ray_end = toBullet(end);

    btCollisionWorld::ClosestRayResultCallback ray_callback(ray_origin, ray_end);
    dynamics_world_->rayTest(ray_origin, ray_end, ray_callback);

    if (ray_callback.hasHit())
    {
      result.hit_id = *(ActorId*)(ray_callback.m_collisionObject->getUserPointer());
      result.hit_position = toGLM(ray_callback.m_hitPointWorld);
      result.hit_normal = toGLM(ray_callback.m_hitNormalWorld);
      result.distance = glm::distance(origin, result.hit_position);
    }

    return result;
  }

  Shape PhysicsEngine::genSphere(float radius)
  {
    Shape shape;
    shape.shape = new btSphereShape(radius);
    shape.volume = (4.f/3.f) * M_PI * radius * radius * radius;

    return shape;
  }

  Shape PhysicsEngine::genBox(const glm::vec3 &dimensions)
  {
    Shape shape;
    shape.shape = new btBoxShape(toBullet(dimensions/2.f));
    shape.volume = dimensions.x * dimensions.y * dimensions.z;

    return shape;
  }

  Shape PhysicsEngine::genCapsule(float radius, float height)
  {
    Shape shape;
    shape.shape = new btCapsuleShape(radius, height);
    shape.volume = ((4.f/3.f) * radius + height) * M_PI * radius * height;

    return shape;
  }

  void PhysicsEngine::internalTickCallback(btDynamicsWorld *world, const btScalar)
  {
    // Check the collisions and store them in the list.
    std::set<CollisionPair> current_collisions;
    btDispatcher *dispatcher = world->getDispatcher();
    for (int id = 0; id < dispatcher->getNumManifolds(); id++)
    {
      const auto *manifold = dispatcher->getManifoldByIndexInternal(id);
      const auto *body_a = static_cast<const btRigidBody *>(manifold->getBody0());
      const auto *body_b = static_cast<const btRigidBody *>(manifold->getBody1());

      const bool swapped = body_a > body_b;
      const btRigidBody *sorted_body_a = swapped ? body_b : body_a;
      const btRigidBody *sorted_body_b = swapped ? body_a : body_b;

      const CollisionPair collision = std::make_pair(sorted_body_a, sorted_body_b);
      current_collisions.insert(collision);

      if (previous_collisions_.find(collision) == previous_collisions_.end())
      {
        // ActorId id_a = *(ActorId*)body_a->getUserPointer();
        // ActorId id_b = *(ActorId*)body_b->getUserPointer();
        // LOG_DEBUG("Collision between %s and %s!", id_a.c_str(), id_b.c_str());
      }
    }

    std::set<CollisionPair> removed_collisions;
    std::set_difference(previous_collisions_.begin(), previous_collisions_.end(),
                        current_collisions.begin(), current_collisions.end(),
                        std::inserter(removed_collisions, removed_collisions.begin()));

    for (auto collision : removed_collisions)
    {
      UNUSED(collision);
      // LOG_DEBUG("No more collision!");
    }

    previous_collisions_ = current_collisions;
  }

  void ActorMotionState::getWorldTransform(btTransform &transform) const
  {
    transform = PhysicsEngine::toBullet(position_, orientation_);
  }

  void ActorMotionState::setWorldTransform(const btTransform &transform)
  {
    position_ = PhysicsEngine::toGLM(transform.getOrigin());
    orientation_ = PhysicsEngine::toGLM(transform.getRotation());
  }

  void ActorMotionState::getWorldTransform(
    glm::vec3 &position, glm::quat &orientation) const
  {
    orientation = orientation_;
    position = position_;
  }

  void ActorMotionState::setWorldTransform(
    const glm::vec3 &position, const glm::quat &orientation)
  {
    orientation_ = orientation;
    position_ = position;
  }
}
