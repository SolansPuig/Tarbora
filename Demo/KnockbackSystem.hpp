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

#ifndef __KNOCKBACKSYSTEM_H_
#define __KNOCKBACKSYSTEM_H_

#include "../Tarbora/Logic/Component.hpp"

namespace Tarbora {
  class KnockbackComponent : public Component {
  public:
    KnockbackComponent (const ActorId &id, const LuaTable &table);
    virtual ~KnockbackComponent () {}

    //! Return the type name of the component, "knockback".
    // For internal use only.
    static ComponentId getStaticType() { return "knockback"; }
    //! Return the type name of the component, "knockback".
    // For internal use only.
    virtual ComponentId getType() const override { return "knockback"; }

    LuaTable properties{LuaTable()};
  };

  class KnockbackUserComponent : public Component {
  public:
    KnockbackUserComponent(const ActorId &id, const LuaTable &table);
    virtual ~KnockbackUserComponent() {}

    //! Return the type name of the component, "knockback_user".
    // For internal use only.
    static ComponentId getStaticType() { return "knockback_user"; }
    //! Return the type name of the component, "knockback_user".
    // For internal use only.
    virtual ComponentId getType() const override { return "knockback_user"; }

    //! The strength of the knockback.
    float strength;
    //! The maximum distance from a target to hit it
    float range;
  };

  class KnockbackSystem : public System {
    friend class World;
  public:
    KnockbackSystem(World *w);

  private:
    void cast(const MessageSubject &subject, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __KNOCKBACKSYSTEM_H_
