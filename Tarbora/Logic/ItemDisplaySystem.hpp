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

#ifndef __ITEMDISPLAYSYSTEM_H_
#define __ITEMDISPLAYSYSTEM_H_

#include "Component.hpp"
#include "InventorySystem.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::ItemDisplayComponent, \ref Tarbora::ItemDisplaySystem
 */

namespace Tarbora {
  struct DisplayColumn
  {
    float column;
    InventorySlot slot;
  };

  struct DisplayRow
  {
    float row;
    std::vector<DisplayColumn> columns;
  };

  class ItemDisplayComponent : public Component
  {
  public:
    ItemDisplayComponent(const ActorId &id, const LuaTable &table);
    virtual ~ItemDisplayComponent() {}

    //! Return the name of that component, "item_display". For internal use only.
    static ComponentId getStaticType() { return "item_display"; }
    //! Return the name of that component, "item_display". For internal use only.
    virtual ComponentId getType() const override { return "item_display"; }

    std::vector<DisplayRow> rows;
  };

  typedef std::shared_ptr<ItemDisplayComponent>  ItemDisplayComponentPtr;

  class ItemDisplaySystem : public System
  {
    friend class World;
  public:
    ItemDisplaySystem(World *w);

  private:
    void interact(const MessageSubject &, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __ITEMDISPLAYSYSTEM_H_
