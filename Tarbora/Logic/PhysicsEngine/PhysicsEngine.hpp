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

#ifndef __PHYSICSENGINE_H_
#define __PHYSICSENGINE_H_

#include "Rigidbody.hpp"
#include "Constraint.hpp"

namespace Tarbora {
  struct RaycastResult {
    ActorId hit_id{""};
    glm::vec3 hit_position{glm::vec3(0.f)};
    glm::vec3 hit_normal{glm::vec3(0.f)};
    float distance{0.f};
  };

  class PhysicsEngine {
  public:
    static bool init();
    static void close();

    static void update(float delta_time);

    static void registerRigidbody(Rigidbody *body);
    static void deleteRigidbody(Rigidbody *body);

    static void registerConstraint(Constraint *constraint);
    static void deleteConstraint(Constraint *constraint);

    static RaycastResult raycast(const glm::vec3 &origin, const glm::vec3 &end);

    static Shape genSphere(float radius);
    static Shape genBox(const glm::vec3 &dimensions);
    static Shape genCapsule(float radius, float height);

    inline static glm::vec3 toGLM(const btVector3& v)
    {
      return glm::vec3(v.getX(), v.getY(), v.getZ());
    }

    inline static glm::vec4 toGLM(const btVector4& v)
    {
      return glm::vec4(v.getX(), v.getY(), v.getZ(), v.getW());
    }

    inline static glm::quat toGLM(const btQuaternion& q)
    {
      return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ());
    }

    inline static glm::mat3 toGLM(const btMatrix3x3 &m)
    {
      return glm::mat3(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
      );
    }

    inline static glm::mat4 toGLM(const btTransform &m)
    {
      const btMatrix3x3 &r = m.getBasis();
      const btVector3 &p = m.getOrigin();

      return glm::mat4(
        r[0][0], r[1][0], r[2][0], 0.f,
        r[0][1], r[1][1], r[2][1], 0.f,
        r[0][2], r[1][2], r[2][2], 0.f,
        p[0],    p[1],    p[2],    1.f
      );
    }

    inline static btVector3 toBullet(const glm::vec3& v)
    {
      return btVector3(v.x, v.y, v.z);
    }

    inline static btVector4 toBullet(const glm::vec4& v)
    {
      return btVector4(v.x, v.y, v.z, v.w);
    }

    inline static btQuaternion toBullet(const glm::quat& q)
    {
      return btQuaternion(q.x, q.y, q.z, q.w);
    }

    inline static btMatrix3x3 toBullet(const glm::mat3& m)
    {
      return btMatrix3x3(
        m[0][0], m[1][0], m[2][0],
        m[0][1], m[1][1], m[2][1],
        m[0][2], m[1][2], m[2][2]
      );
    }

    inline static btTransform toBullet(
      const glm::vec3 &position, const glm::quat &rotation)
    {
      return btTransform(toBullet(rotation), toBullet(position));
    }

  private:
    static void internalTickCallback(
      btDynamicsWorld *world, const btScalar delta_time);

    static void deleteObject(btCollisionObject *object);
       
    // Bullet3 variables
    inline static btDynamicsWorld *dynamics_world_;
    inline static btBroadphaseInterface *broadphase_;
    inline static btCollisionDispatcher *dispatcher_;
    inline static btConstraintSolver *solver_;
    inline static btDefaultCollisionConfiguration *collision_configuration_;

    // List of the current collisions
    typedef std::pair<btRigidBody const *, btRigidBody const *> CollisionPair;
    inline static std::set<CollisionPair> previous_collisions_;
  };

  //! The position and rotation of an actor.
  class ActorMotionState : public btMotionState
  {
  public:
    ActorMotionState(const glm::vec3 &position, const glm::quat &orientation)
      : position_(position), orientation_(orientation) {}

    virtual void getWorldTransform(btTransform &transform) const;
    virtual void setWorldTransform(const btTransform &transform);
    void getWorldTransform(glm::vec3 &position, glm::quat &orientation) const;
    void setWorldTransform(const glm::vec3 &position, const glm::quat &orientation);

    const glm::vec3& getPosition() { return position_; }
    const glm::quat& getOrientation() { return orientation_; }

    glm::vec3 position_;
    glm::quat orientation_;
  };
}

#endif // __PHYSICSENGINE_H_
