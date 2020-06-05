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

#include "World.hpp"
#include "InventorySystem.hpp"
#include "ControllerSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"
#include "AnimationSystem.hpp"
#include "EntitySystem.hpp"

namespace Tarbora {
  InventoryGroup::InventoryGroup(
    unsigned int max_slots,
    bool visible,
    const std::string &node,
    const LuaTable &offsets,
    const LuaTable &orientations
  ) : visible(visible), node(node)
  {
    for (unsigned int i = 1; i <= max_slots; i++)
    {
      auto offset = offsets.get(i, glm::vec3(0.f), true);
      auto orientation = orientations.get(i, glm::vec3(0.f), true);
      slots.emplace_back(
        InventorySlot(
          ItemComponentPtr(), 0, offset, glm::quat(glm::radians(orientation))
        )
      );
    }
  }

  InventoryComponent::InventoryComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    for (auto g : table)
    {
      LuaTable group = g.second.getAs<LuaTable>();
      std::string name = group.get<std::string>("name");
      unsigned int slots = group.get<unsigned int>("slots", 1, true);
      std::string node = group.get<std::string>("node", "", true);
      LuaTable offsets = group.get("offsets", true);
      LuaTable orientations = group.get("orientations", true);
      inventory.emplace(
        name,
        InventoryGroup(slots, node != "", node, offsets, orientations)
      );
    }
  }

  ItemComponent::ItemComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    name = table.get<std::string>("name", "");
    description = table.get<std::string>("description", "", true);
    max_quantity = table.get<int>("max_quantity", 1, true);
    offset = table.get<glm::vec3>("offset", true);
    orientation = glm::quat(glm::radians(table.get<glm::vec3>("orientation", true)));

    for (auto group : table.get("valid_groups", true))
    {
      valid_groups.push_back(group.second.getAs<std::string>());
    }
    if (valid_groups.empty())
    {
      valid_groups.push_back("INVENTORY");
    }
  }

  InventorySystem::InventorySystem(World *w) :
    System(w)
  {
    components->registerFactory("inventory", [&](auto id, auto table)
    {
      return std::make_shared<InventoryComponent>(id, table);
    });
    components->registerFactory("item", [&](auto id, auto table)
    {
      return std::make_shared<ItemComponent>(id, table);
    });
    components->onEnable("inventory", ENBIND(&InventorySystem::enableInventory));
    components->onDisable("inventory", [&](auto comp)
    {
      trigger("inventory_disable", Message::Actor(comp->owner));
      return true;
    });

    subscribe("pick_item", MSGBIND(&InventorySystem::pick));
    subscribe("drop_item", MSGBIND(&InventorySystem::drop));
  }

  ItemComponentPtr InventorySystem::getItem(InventoryComponentPtr inv)
  {
    auto sight = components->getComponent<SightComponent>(inv->owner);
    if (sight && sight->enabled())
    {
      auto item = components->getComponent<ItemComponent>(sight->target);
      return (item && item->enabled()) ? item : ItemComponentPtr();
    }
    return ItemComponentPtr();
  }

  void InventorySystem::pick(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m->id();

    auto inv = components->getComponent<InventoryComponent>(id);
    if (inv && inv->enabled())
    {
      auto item = getItem(inv);
      if (item)
      {
        // Try to fit the item in every valid group
        for (auto vgroup : item->valid_groups)
        {
          auto group = inv->inventory.find(vgroup);
          if (group != inv->inventory.end())
          {
            auto slots = &group->second.slots;
            unsigned int slot_number = 0;
            unsigned int count = 0;
            auto slot = slots->end();
            auto first_free = slots->end();

            // Find a suitable slot. One containgin items with the same name, or an
            // empty one.
            for (auto itr = slots->begin(); itr != slots->end(); itr++)
            {
              if (itr->item)
              {
                if (itr->item->name == item->name && itr->quantity < item->max_quantity)
                {
                  // No duplicates. Items of the same type get destroyed.
                  world->deleteActor(item->owner);
                  itr->quantity++;
                  slot = itr;
                  slot_number = count;
                  // Forget the first free slot. No longer needed.
                  first_free = slots->end();
                  break;
                }
              }
              else if (first_free == slots->end())
              {
                // Found the first empty slot. Remember it but keep searching,
                // slots with items of the same type take precedence.
                first_free = itr;
                slot_number = count;
              }
              count++;
            }

            if (first_free != slots->end())
            {
              // A free slot was found, store the item to it.
              world->disableActor(item->owner);
              first_free->item = item;
              first_free->quantity = 1;
              slot = first_free;

              if (group->second.visible)
              {
                auto model = components->getComponent<ModelComponent>(item->owner);
                if (model)
                {
                  model->parent = id;
                  model->parent_node = group->second.node;
                  model->offset = slot->offset + slot->orientation * item->offset;
                  model->orientation = slot->orientation * item->orientation;
                }
                components->enableComponent<ModelComponent>(item->owner);
                components->enableComponent<AnimationComponent>(item->owner);
              }
            }

            if (slot != slots->end())
            {
              // The item was picked. Send the message.
              Message::Item msg(id, group->first, slot_number, slot->quantity);
              msg->set_target(item->owner);
              msg->set_name(slot->item->name);
              msg->set_description(slot->item->name);
              trigger("picked_item", msg);

              break;
            }
          }
        }
      }
    }
  }

  void InventorySystem::drop(const MessageSubject &, const MessageBody &body)
  {
    Message::Item m(body);
    ActorId id = m->owner();

    auto inv = components->getComponent<InventoryComponent>(id);
    if (inv && inv->enabled())
    {
      // Get the dropped item
      ActorId target = "";
      auto group = inv->inventory.find(m->inv_group());
      if (group != inv->inventory.end())
      {
        if (group->second.slots.size() <= m->slot()) return;

        auto slot = &group->second.slots.at(m->slot());
        if (slot->quantity > 1)
        {
          // Drop a clone
          auto transform = components->getComponent<TransformComponent>(id);
          auto info = components->getComponent<InfoComponent>(slot->item->owner);
          if (transform && info)
          {
            target = world->createActor(
              "",
              info->entity,
              transform->position + transform->orientation * glm::vec3(0.f, 0.f, 1.f),
              transform->orientation
            );
            slot->quantity--;
          }
        }
        else if (slot->quantity == 1)
        {
          // Drop it
          auto transform = components->getComponent<TransformComponent>(id);
          auto t_trans = components->getComponent<TransformComponent>(slot->item->owner);
          if (transform)
          {
            target = slot->item->owner;
            t_trans->position = transform->position + transform->orientation *
              glm::vec3(0.f, 0.f, 1.f);
            t_trans->orientation = transform->orientation;

            if (group->second.visible)
            {
              auto model = components->getComponent<ModelComponent>(target);
              if (model)
              {
                model->parent = "";
                model->parent_node = "";
                model->offset = glm::vec3(0.f);
              }
              components->disableComponent<ModelComponent>(target);
              components->disableComponent<AnimationComponent>(target);
            }

            world->enableActor(target);

            slot->quantity = 0;
            slot->item = ItemComponentPtr();
          }
        }

        // Add a bit of forward impulse
        auto rb = components->getComponent<RigidbodyComponent>(target);
        if (rb && rb->enabled())
        {
          rb->applyImpulse({0.f, 0.f, 500.f});
        }

        //  Notify
        if (target != "")
        {
          Message::Item msg(id, m->inv_group(), m->slot(), slot->quantity);
          msg->set_target(target);
          msg->set_name(slot->item ? slot->item->name : "");
          msg->set_description(slot->item ? slot->item->description : "");
          trigger("dropped_item", msg);
        }
      }
    }
  }

  bool InventorySystem::enableInventory(std::shared_ptr<Component> comp)
  {
    auto inv = std::static_pointer_cast<InventoryComponent>(comp);
    Message::Inventory msg(inv->owner);

    for (auto g : inv->inventory)
    {
      MessageContent::Group group;
      for (auto slot : g.second.slots)
      {
        auto item = group.add_slots();
        if (slot.item)
        {
          item->set_name(slot.item->name);
          item->set_description(slot.item->description);
        }
        item->set_quantity(slot.quantity);
      }
      (*msg->mutable_inventory())[g.first] = group;
    }

    trigger("inventory_enable", msg);
    return true;
  }

  void InventorySystem::update(float)
  {
   
  }
}
