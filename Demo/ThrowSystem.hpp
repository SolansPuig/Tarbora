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

#ifndef __THROWSYSTEM_H_
#define __THROWSYSTEM_H_

#include "../Tarbora/Logic/Component.hpp"

namespace Tarbora {
  class ThrowComponent : public Component {
  public:
    ThrowComponent (const ActorId &id, const LuaTable &table);
    virtual ~ThrowComponent () {}

    //! Return the type name of the component, "throw".
    // For internal use only.
    static ComponentId getStaticType() { return "throw"; }
    //! Return the type name of the component, "throw".
    // For internal use only.
    virtual ComponentId getType() const override { return "throw"; }

    LuaTable properties{LuaTable()};
  };

  class ThrowUserComponent : public Component {
  public:
    ThrowUserComponent(const ActorId &id, const LuaTable &table);
    virtual ~ThrowUserComponent() {}

    //! Return the type name of the component, "throw_user".
    // For internal use only.
    static ComponentId getStaticType() { return "throw_user"; }
    //! Return the type name of the component, "throw_user".
    // For internal use only.
    virtual ComponentId getType() const override { return "throw_user"; }

    //! The projectile that is thrown
    ActorId projectile;
    //! The strength at which it is thrown
    float strength;
    //! The offset from the center of the actor from where it is thrown
    glm::vec3 origin;

    //! The active projectiles
    std::queue<ActorId> projectiles;
    //! The max number of projectiles
    unsigned int max_projectiles;
  };

  class ThrowSystem : public System {
    friend class World;
  public:
    ThrowSystem(World *w);

  private:
    void cast(const MessageSubject &subject, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __THROWSYSTEM_H_
