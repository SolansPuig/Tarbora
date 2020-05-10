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

#ifndef __INVENTORYLAYER_H_
#define __INVENTORYLAYER_H_

#include "Layer.hpp"

namespace Tarbora {
  struct Item
  {
    Item() {}

    Item(
      const std::string &name,
      const std::string &description,
      unsigned int quantity
    ) : name(name), description(description), quantity(quantity) {}

    std::string name{""};
    std::string description{""};
    unsigned int quantity{0};
  };

  struct InventoryGroup
  {
    InventoryGroup(unsigned int size)
    {
      slots.resize(size);
    }

    inline auto begin() { return slots.begin(); }
    inline auto end() { return slots.end(); }
    inline auto size() { return slots.size(); }

    std::vector<Item> slots;
    unsigned int used{0};
  };

  struct Inventory
  {
    Inventory();

    void setSlot(
      const std::string &group,
      unsigned int slot,
      const std::string &name,
      const std::string &description,
      unsigned int quantity
    );

    inline auto begin() { return inventory.begin(); }
    inline auto end() { return inventory.end(); }
    inline auto size() { return inventory.size(); }

    ActorId owner;

    std::map<std::string, InventoryGroup> inventory;
  };

  class InventoryLayer : public Layer
  {
  public:
    InventoryLayer(GraphicView *view);
    virtual ~InventoryLayer() {}

    virtual void getInput() override;
    virtual void update(float delta_time) override;
    virtual void draw() override;

  private:
    Inventory main_;
    bool enabled_{false};
  };
}

#endif // __INVENTORYLAYER_H_
