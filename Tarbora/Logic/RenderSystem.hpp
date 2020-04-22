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

#ifndef __RENDERSYSTEM_H_
#define __RENDERSYSTEM_H_

#include "Component.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::RenderSystem and \ref Tarbora::ModelComponent.
 */

namespace Tarbora {
  /**
   * \brief The component that stores the info about rendering.
   *
   * It is created by \ref Tarbora::RenderSystem.
   * */
  class ModelComponent : public Component {
  public:
    ModelComponent(const ActorId &id) : Component(id) {}
    virtual ~ModelComponent() {}

    //! Return the type name of the component, "model". For internal use only.
    static ComponentId getStaticType() { return "model"; }
    //! Return the type name of the component, "model". For internal use only.
    virtual ComponentId getType() const override { return "model"; }

    //! The render pass when the model will be rendered.
    int render_pass;
    //! The file name of the model.
    std::string model;
    //! The file name of the material.
    std::string material;
  };

  /**
   * \brief The system that manages rendering.
   *
   *  It owns one component: \ref Tarbora::ModelComponent.
   *
   *  <b>Triggers:</b>
   *
   * \c create_actor_model (\ref Message::CreateActorModel) after a component creation.
   *
   * \c move_actor (\ref Message::MoveActor) after a component creation.
   */
  class RenderSystem : public System {
    friend class World;
  public:
    RenderSystem(World *w);

  private:
    // Factories
    ComponentPtr modelFactory(const ActorId &id, const LuaTable &table);

    virtual void update(float delta_time) override;
  };
}

#endif // __RENDERSYSTEM_H_
