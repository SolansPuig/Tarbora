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

#include "InventoryLayer.hpp"
#include "../Tarbora/Messages/BasicMessages.hpp"

namespace Tarbora {
  Inventory::Inventory() {}

  void Inventory::setSlot(
    const std::string &group,
    unsigned int slot,
    const std::string &name,
    const std::string &description,
    unsigned int quantity
  )
  {
    auto g = inventory.find(group);
    if (g != inventory.end())
    {
      if (slot < g->second.size())
      {
        unsigned int old_quantity = g->second.slots[slot].quantity;
        g->second.slots[slot] = Item(name, description, quantity);

        if (old_quantity == 0 && quantity >= 1)
          g->second.used++;
        else if (old_quantity >= 1 && quantity == 0)
          g->second.used--;
      }
    }
  }

  InventoryLayer::InventoryLayer(HumanView *view) :
    Layer(view, false)
  {
    subscribe("inventory_enable", [&](const MessageSubject &, const MessageBody &body)
    {
      Message::Inventory m(body);
      if (m->id() == "player")
      {
        enabled_ = true;
        for (auto g : m->inventory())
        {
          auto group = main_.inventory.find(g.first);
          if (group == main_.inventory.end())
          {
            group = main_.inventory.emplace(
              g.first,
              InventoryGroup(g.second.slots_size())
            ).first;
          }
          for (auto s : g.second.slots())
          {
            main_.setSlot(g.first, s.slot(), s.name(), s.description(), s.quantity());
          }
        }
      }
    });

    subscribe("inventory_disable", [&](const MessageSubject &, const MessageBody &body)
    {
      Message::Actor m(body);
      if (m->id() == "player")
      {
        enabled_ = false;
      }
    });

    subscribe("picked_item", [&](const MessageSubject &, const MessageBody &body)
    {
      Message::Item m(body);
      if (m->owner() == "player")
      {
        main_.setSlot(
          m->inv_group(), m->slot(), m->name(), m->description(), m->quantity());
      }
    });

    subscribe("dropped_item", [&](const MessageSubject &, const MessageBody &body)
    {
      Message::Item m(body);
      if (m->owner() == "player")
      {
        main_.setSlot(
          m->inv_group(), m->slot(), m->name(), m->description(), m->quantity());
      }
    });
  }

  bool InventoryLayer::getInput()
  {
    if (getInputManager()->getKeyDown(KEY_I))
    {
      active_ = enabled_ && !active_;
    }

    return true;
  }

  void InventoryLayer::update(float delta_time)
  {
    UNUSED(delta_time);
  }

  void InventoryLayer::draw()
  {
    ImGui::Begin("Character Inventory", &active_, ImGuiWindowFlags_None);

    for (auto group : main_)
    {
      ImGui::PushID(group.first.c_str());

      if (group.second.size() > 1)
      {
        ImGui::Text(
          "%s\t\tUsed: %u of %lu",
          group.first.c_str(),
          group.second.used,
          group.second.size()
        );
      }
      else
      {
        ImGui::Text("%s", group.first.c_str());
      }

      ImGui::Separator();

      unsigned int n = 0;
      for (auto slot : group.second)
      {
        if (slot.quantity != 0)
        {
          ImGui::PushID(n);
         
          if (slot.quantity > 1)
            ImGui::Text("%s (%u)", slot.name.c_str(), slot.quantity);
          else
            ImGui::Text("%s", slot.name.c_str());

          ImGui::SameLine(400.f);

          if (ImGui::Button("Drop"))
          {
            send(1, "drop_item", Message::Item("player", group.first, n, 1));
          }

          ImGui::PopID();
        }

        n++;
      }

      ImGui::PopID();
    }


    ImGui::SetItemDefaultFocus();
    ImGui::End();
  }
}
