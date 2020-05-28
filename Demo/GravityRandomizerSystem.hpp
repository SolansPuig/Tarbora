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

#ifndef __GRAVITYRANDOMIZERSYSTEM_H_
#define __GRAVITYRANDOMIZERSYSTEM_H_

#include "../Tarbora/Logic/Component.hpp"

namespace Tarbora {
  class GravityRandomizerComponent : public Component {
  public:
    GravityRandomizerComponent (const ActorId &id, const LuaTable &table);
    virtual ~GravityRandomizerComponent () {}

    //! Return the type name of the component, "gravity_randomizer".
    // For internal use only.
    static ComponentId getStaticType() { return "gravity_randomizer"; }
    //! Return the type name of the component, "gravity_randomizer".
    // For internal use only.
    virtual ComponentId getType() const override { return "gravity_randomizer"; }

    LuaTable properties{LuaTable()};
  };

  class GravityRandomizerUserComponent : public Component {
  public:
    GravityRandomizerUserComponent(const ActorId &id, const LuaTable &table);
    virtual ~GravityRandomizerUserComponent() {}

    //! Return the type name of the component, "gravity_randomizer_user".
    // For internal use only.
    static ComponentId getStaticType() { return "gravity_randomizer_user"; }
    //! Return the type name of the component, "gravity_randomizer_user".
    // For internal use only.
    virtual ComponentId getType() const override { return "gravity_randomizer_user"; }

    //! The maximum distance from a target to hit it
    float range;
  };

  class GravityRandomizerSystem : public System {
    friend class World;
  public:
    GravityRandomizerSystem(World *w);

  private:
    void cast(const MessageSubject &subject, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __GRAVITYRANDOMIZERSYSTEM_H_
