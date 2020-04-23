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

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "../Framework/Framework.hpp"
#include "../Messages/BasicMessages.hpp"

/**
 * \file
 * \brief Class \ref Tarbora::System.
 */

namespace Tarbora {
  class World;
  class ComponentManager;

  /**
   * \brief A pure virtual system for the \ref Entity_Component_System
   *
   * All systems should inherit from this.
   */
  class System : public ModuleComponent {
    friend class World;
  public:
    System(World *w);

    World *world;
    ComponentManager *components;
  protected:
    //! Perform all the actions for the components related to that system.
    /*!
     * \param delta_time The time, in seconds, since the last update.
     */
    virtual void update(float delta_time) = 0;
  };
}

#endif
