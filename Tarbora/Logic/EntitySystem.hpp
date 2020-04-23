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

#ifndef __TYPESYSTEM_H_
#define __TYPESYSTEM_H_

#include "Component.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::EntitySystem, \ref Tarbora::TypeComponent,
 * \ref Tarbora::InfoComponent and \ref Tarbora::TransformComponent.
 */

namespace Tarbora {
  class EntitySystem;
 
  /**
   * \brief The component that stores the list of types of an actor.
   *
   * This component doesn't have a specific usage, instead is meant to be used for
   * together with other components.
   *
   * It is created by \ref Tarbora::EntitySystem.
   */
  class TypeComponent : public Component {
  public:
    TypeComponent(const ActorId &id, const LuaTable &table);
    virtual ~TypeComponent() {}

    //! Return the type name of that component, "type". For internal use only.
    static ComponentId getStaticType() { return "type"; }
    //! Return the type name of that component, "type". For internal use only.
    virtual ComponentId getType() const override { return "type"; }

    //! Add a new type.
    void add(const std::string &type);
    //! Check if the actor has a certain type.
    bool has(const std::string &type);

    std::vector<std::string> types;
  };

  /**
   * \brief The component that stores the basic info of an actor.
   *
   * Currently it's only used to store the entity type and variant names.
   *
   * It is created by \ref Tarbora::EntitySystem.
   */
  class InfoComponent : public Component {
  public:
    InfoComponent(const ActorId &id, const LuaTable &table);
    virtual ~InfoComponent() {}

    //! Return the type name of that component, "info". For internal use only.
    static ComponentId getStaticType() { return "info"; }
    //! Return the type name of that component, "info". For internal use only.
    virtual ComponentId getType() const override { return "info"; }

    std::string entity;
    std::string variant;
  };

  /**
   * \brief The component that stores the current position and orientation of an actor
   *
   * It is created by \ref Tarbora::EntitySystem.
   */
  class TransformComponent : public Component {
  public:
    /**
     * \brief Constructor.
     *
     * \param id The id of the actor that owns that component.
     */
    TransformComponent(const ActorId &id, const LuaTable &table);
    virtual ~TransformComponent() {}

    //! Return the type name of that component, "transform". For internal use only.
    static ComponentId getStaticType() { return "transform"; }
    //! Return the type name of that component, "transform". For internal use only.
    virtual ComponentId getType() const override { return "transform"; }

    glm::vec3 position;
    glm::quat orientation;
    bool update{false};
  };

  /**
   * \brief The system that manages the basic info about actors.
   *
   * It owns three components: \ref Tarbora::TransformComponent, \ref
   * Tarbora::InfoComponent and \ref Tarbpora::TypeComponent.
   *
   * <b>Subscribed to:</b>
   *
   * \c set_position (Message::ApplyPhysics). Sets the actor to a new position.
   *
   * \c set_orientation (Message::ApplyPhysics). Sets the actor to a new orientation.
   *
   * \c move (Message::ApplyPhysics). Increments the actor's current position.
   *
   * \c rotate (Message::ApplyPhysics). Increments the actor's current orientation.
   *
   * <b>Triggers:</b>
   *
   * \c move_actor (Message::MoveActor) every frame for every actor whose transform
   * changed during that frame.
   * */
  class EntitySystem : public System {
    friend class World;
  public:
    EntitySystem(World *w);

  private:
    // Factories
    ComponentPtr typeFactory(const ActorId &id, const LuaTable &table);
    ComponentPtr transformFactory(const ActorId &id, const LuaTable &table);
    ComponentPtr infoFactory(const ActorId &id, const LuaTable &table);

    // Message subscriptions for transform
    void setPosition(const MessageSubject &, const MessageBody &body);
    void setOrientation(const MessageSubject &, const MessageBody &body);
    void move(const MessageSubject &, const MessageBody &body);
    void rotate(const MessageSubject &, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __TYPESYSTEM_H_
