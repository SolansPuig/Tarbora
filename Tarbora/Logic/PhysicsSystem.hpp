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

#ifndef __PHYSICSSYSTEM_H_
#define __PHYSICSSYSTEM_H_

#include "Component.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::PhysicsSystem and \ref Tarbora::PhysicsComponent
 */

namespace Tarbora {
  class PhysicsSystem;
 
  /**
   * \brief The component that stores the info about a physics' \ref Tarbora::Rigidbody.
   *
   * It is created by \ref Tarbora::PhysicsSystem.
   */
  class RigidbodyComponent : public Component, public Rigidbody {
  public:
    RigidbodyComponent(const ActorId &id, const LuaTable &table);
    virtual ~RigidbodyComponent() {}

    //! Return the type name of the component, "rigidboy". For internal use only.
    static ComponentId getStaticType() { return "rigidbody"; }
    //! Return the type name of the component, "rigidboy". For internal use only.
    virtual ComponentId getType() const override { return "rigidbody"; }
  };

  /**
   * \brief The system that manages physics.
   *
   * It owns one component: \ref Tarbora::PhysicsComponent.
   *
   * <b>Subscribed to:</b>
   *
   * \c set_position (\ref Message::ApplyPhysics). Sets the actor to a new position.
   *
   * \c set_orientation (\ref Message::ApplyPhysics). Sets the actor to a new orientation.
   *
   * \c move (\ref Message::ApplyPhysics). Increments the actor's current position.
   *
   * \c rotate (\ref Message::ApplyPhysics). Increments the actor's current orientation.
   *
   * \c apply_force (\ref Message::ApplyPhysics). Applies a linear force at the center
   * of the actor.
   *
   * \c apply_torque (\ref Message::ApplyPhysics). Applies an angular force at the center
   * of the actor.
   *
   * \c set_velocity (\ref Message::ApplyPhysics). Sets the actor's velocity. This
   * overrides the current velocity, and the effects of forces, gravity included. A value
   * of 0 will freeze the actor in place, until a new force, like gravity, is applied.
   *
   * \c set_angular_vel (\ref Message::ApplyPhysics). Sets the actor's angular velocity,
   * a rotation velocity. Like with \c set_velocity, this will override all angular
   * velocities taking place, until new ones are applied.
   *
   * \c stop (\ref Message::ApplyPhysics). Has the same effect as a \c set_velocity with a
   * value of 0.
   *
   * See also \ref Tarbora::PhysicsEngine, \ref Tarbora::Rigidbody,
   * \ref Tarbora::ControllerSystem.
   * */
  class PhysicsSystem : public System {
    friend class World;
  public:
    PhysicsSystem(World *w);

  private:
    // Factories
    ComponentPtr physicsFactory(const ActorId &id, const LuaTable &table);

    // Message subscriptions
    void init(const MessageSubject &, const MessageBody &body);
    void setPosition(const MessageSubject &, const MessageBody &body);
    void setOrientation(const MessageSubject &, const MessageBody &body);
    void move(const MessageSubject &, const MessageBody &body);
    void rotate(const MessageSubject &, const MessageBody &body);
    void applyForce(const MessageSubject &, const MessageBody &body);
    void applyTorque(const MessageSubject &, const MessageBody &body);
    void setVelocity(const MessageSubject &, const MessageBody &body);
    void setAngularVel(const MessageSubject &, const MessageBody &body);
    void stop(const MessageSubject &, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __PHYSICSSYSTEM_H_
