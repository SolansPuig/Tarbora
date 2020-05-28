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

#ifndef __RIGIDBODY_H_
#define __RIGIDBODY_H_

#include "../../Framework/CommonHeaders.hpp"
#include "../../Framework/Global.hpp"
#include "Bullet.hpp"

namespace Tarbora {
  struct RaycastResult;

  struct Shape {
    float volume;
    btCollisionShape *shape;
  };

  class Rigidbody {
    friend class PhysicsEngine;
    friend class Constraint;
    friend class PointConstraint;

  public:
    Rigidbody(const ActorId &id);
    ~Rigidbody();

    void create(const glm::vec3 &position, const glm::quat &orientation);
    void destroy();

    void setTransform(const glm::vec3 &position, const glm::quat &orientation);
    void setPosition(const glm::vec3 &position);
    void move(const glm::vec3 &direction);
    void setOrientation(const glm::quat &orientation);
    const glm::vec3& getPosition();
    const glm::quat& getOrientation();

    glm::mat4 getLocalTransform();
    glm::mat4 getWorldTransform();

    void applyForce(const glm::vec3 &vector);
    void applyTorque(const glm::vec3 &vector);
    void applyImpulse(const glm::vec3 &impulse);
    void applyImpulse(const glm::vec3 &impulse, const glm::vec3 &position);
    void setVelocity(const glm::vec3 &vector);
    void setRotation(const glm::vec3 &vector);

    void setFriction(float value);
    void setDensity(float value);
    void setResitution(float value);

    void setGravity(const glm::vec3 &vector);

    RaycastResult raycast(
      const glm::vec3 &origin,
      const glm::vec3 &direction,
      float distance
    );

    void setDamping(float linear, float angular);
    void setAngularFactor(const glm::vec3 &axis);

    bool isStatic();

    void alwaysActive(bool always);

    //! The shape of the rigidbody.
    Shape shape;
    //! The height, in meters, of the rigidbody.
    float height;
    //! The friction between the body and other objects.
    float friction;
    //! The density, which will be used to calculate the mass.
    float density;
    //! The mass of the rigidbody.
    float mass;
    // The bounciness of the rigidbody.
    float restitution;

  private:
    ActorId user_pointer_;
    std::shared_ptr<btRigidBody> body_;
    bool enabled_{false};
  };
}

#endif // __RIGIDBODY_H_
