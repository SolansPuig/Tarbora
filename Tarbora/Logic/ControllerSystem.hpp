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

#ifndef __CONTROLLERSYSTEM_H_
#define __CONTROLLERSYSTEM_H_

#include "Component.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::ControllerSystem, \ref Tarbora::ControllerComponent,
 * \ref Tarbora::SightComponent and \ref Tarbora::GrabComponent.
 */

namespace Tarbora {
  /**
   * \brief The component that stores all the info about the actor movement and related.
   *
   * This component is used for movement, track the head position and check if the entity
   * is on the ground or falling.
   *
   * It is created by \ref Tarbora::ControllerSystem.
   * */
  class ControllerComponent : public Component {
  public:
    ControllerComponent(const ActorId &id) : Component(id) {}
    virtual ~ControllerComponent() {}

    //! Return the type name of that component, "controller". For internal use only.
    static ComponentId getStaticType() { return "controller"; }
    //! Return the type name of that component, "controller". For internal use only.
    virtual ComponentId getType() const override { return "controller"; }

    //! The speed multiplier when walking.
    float speed;
    //! The speed multiplier when running.
    float run_speed;
    //! The multiplier for rotation.
    float rotation_speed;
    /**
     * \brief The limits for the \c facing vector, in degrees.
     *
     * Defaults to (70.0, 70.0, 70.0).
     *
     * The maximum value for \c facing is \c facing_clamp and the minimum is
     * \c -facing_clamp.
     */
    glm::vec3 facing_clamp;
    //! The direction where it's moving.
    glm::vec3 movement{glm::vec3(0.f)};
    //! The rotation direction.
    glm::vec3 rotation{glm::vec3(0.f)};
    //! The direction where it's facing.
    glm::vec3 facing{glm::vec3(0.f)};
    bool on_ground{false};
    bool walking{false};
  };

  /**
   * \brief This component checks and stores where the actor is looking at.
   *
   * Caution: it triggers a raycast check every frame.
   *
   * It is created by \ref Tarbora::ControllerSystem.
   */
  class SightComponent : public Component {
  public:
    SightComponent(const ActorId &id) : Component(id) {}
    virtual ~SightComponent() {}

    //! Return the type name of that component, "sight". For internal use only.
    static ComponentId getStaticType() { return "sight"; }
    //! Return the type name of that component, "sight". For internal use only.
    virtual ComponentId getType() const override { return "sight"; }

    /**
     * \brief The starting point of the raycast. In local coordinates.
     *
     * It must be in local coordinates, and defaults to (0.0, 0.0, 0.0).
     */
    glm::vec3 eye_position;
    /**
     * \brief The maximum distance of the raycast.
     *
     * Defaults to 10.0
     * */
    float look_distance;
    //! The direction where the actor is facing. Updated by the controller system.
    glm::quat look_direction{glm::quat()};

    //! The Id entity that this entity is looking at (where the raycast hits).
    ActorId target{""};
    //! The distance from the eye position to the point where the raycast hits.
    float target_distance{0.f};
    //! Where the raycast hits, in global coordinates.
    glm::vec3 target_position{glm::vec3(0.f)};
  };

  /**
   * \brief This component allows the actor to grab and move another actor.
   */
  class GrabComponent : public Component {
  public:
    GrabComponent(const ActorId &id) : Component(id) {}
    virtual ~GrabComponent() {}

    //! Return the type name of that component, "grab". For internal use only.
    static ComponentId getStaticType() { return "grab"; }
    //! Return the type name of that component, "grab". For internal use only.
    virtual ComponentId getType() const override { return "grab"; }

    //! The grabbed actor. If it's empty, nothing is grabbed.
    ActorId target{""};
    /**
     * \brief The distance between this actor and the target.
     *
     * Clamped to a minimum of 1.0 meters.
     */
    float distance{1.f};
    //! \brief The local position where the target is grabbed.
    glm::vec3 position{glm::vec3(0.f)};
    float grid{0.f};
    bool enable_grid{false};

    std::shared_ptr<PointConstraint> constraint;
  };

  class ControllerSystem : public System {
    friend class World;
  public:
    ControllerSystem(World *w);

  private:
    // Factories
    ComponentPtr controllerFactory(const ActorId &id, const LuaTable &table);
    ComponentPtr sightFactory(const ActorId &id, const LuaTable &table);
    ComponentPtr grabFactory(const ActorId &id, const LuaTable &table);

    // Message subscriptions for controller
    void setMovement(const MessageSubject &, const MessageBody &body);
    void setRotation(const MessageSubject &, const MessageBody &body);
    void setFacing(const MessageSubject &, const MessageBody &body);
    // Message subscriptiosn for grab
    void grabObject(const MessageSubject &, const MessageBody &body);
    void releaseObject(const MessageSubject &, const MessageBody &body);
    void grabDistance(const MessageSubject &, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __CONTROLLERSYSTEM_H_
