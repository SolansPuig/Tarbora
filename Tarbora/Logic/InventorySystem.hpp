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

#ifndef __INVENTORYSYSTEM_H_
#define __INVENTORYSYSTEM_H_

#include "Component.hpp"

/**
 * \file
 * \brief Classes \ref Tarbora::InventoryComponent, \ref Tarbora::ItemComponent,
 * \ref Tarbora::InventorySystem.
 */

namespace Tarbora {
  class ItemComponent : public Component
  {
  public:
    ItemComponent(const ActorId &id, const LuaTable &table);
    virtual ~ItemComponent() {}

    //! Return the type name of that component, "item". For internal use only.
    static ComponentId getStaticType() { return "item"; }
    //! Return the type name of that component, "item". For internal use only.
    virtual ComponentId getType() const override { return "item"; }

    std::string name;
    std::string description;
    unsigned int max_quantity;
    std::vector<std::string> valid_groups;
    glm::vec3 offset;
    glm::quat orientation;
  };

  typedef std::shared_ptr<ItemComponent> ItemComponentPtr;

  struct InventorySlot
  {
    InventorySlot (
      ItemComponentPtr item,
      unsigned int quantity,
      const glm::vec3 &offset,
      const glm::quat &orientation
    )
      : item(item), quantity(quantity), offset(offset), orientation(orientation) {}
  
    ItemComponentPtr item;
    unsigned int quantity;
    glm::vec3 offset;
    glm::quat orientation;
  };

  struct InventoryGroup
  {
    InventoryGroup(
      unsigned int max_slots,
      bool visible,
      const std::string &node,
      const LuaTable &offsets,
      const LuaTable &orientations
    );

    std::vector<InventorySlot> slots;
    bool visible;
    std::string node;
  };

  class InventoryComponent : public Component
  {
  public:
    InventoryComponent(const ActorId &id, const LuaTable &table);
    virtual ~InventoryComponent() {}

    //! Return the type name of that component, "inventory". For internal use only.
    static ComponentId getStaticType() { return "inventory"; }
    //! Return the type name of that component, "inventory". For internal use only.
    virtual ComponentId getType() const override { return "inventory"; }

    std::unordered_map<std::string, InventoryGroup> inventory;
  };

  typedef std::shared_ptr<InventoryComponent> InventoryComponentPtr;

  class InventorySystem : public System
  {
    friend class World;
  public:
    InventorySystem(World *w);

  private:
    bool enableInventory(std::shared_ptr<Component> comp);
   
    //  Message subscriptions for inventory
    ItemComponentPtr getItem(InventoryComponentPtr inv);
    void pick(const MessageSubject &, const MessageBody &body);
    void drop(const MessageSubject &, const MessageBody &body);

    virtual void update(float delta_time) override;
  };
}

#endif // __INVENTORYSYSTEM_H_
