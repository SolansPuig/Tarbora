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

#include "ItemDisplaySystem.hpp"
#include "ControllerSystem.hpp"
#include "PhysicsSystem.hpp"
#include "World.hpp"
#include "RenderSystem.hpp"
#include "AnimationSystem.hpp"

namespace Tarbora {
  ItemDisplayComponent::ItemDisplayComponent(const ActorId &id, const LuaTable &table) :
    Component(id, table)
  {
    unsigned int items = 0;
    LuaTable offsets = table.get("offsets", true);
    LuaTable orientations = table.get("orientations", true);

    for (auto r : table.get("items", true))
    {
      auto row = r.second.getAs<LuaTable>();
      auto display = rows.emplace(rows.end());
      display->row = row.get<float>("row", 1.f, true);
      for (auto c : row.get("columns", true))
      {
        auto column = display->columns.emplace(display->columns.end());
        column->column = c.second.getAs<float>();
        items++;
        if (offsets.valid())
          column->slot.offset = offsets.get<glm::vec3>(items);
        if (orientations.valid())
          column->slot.orientation = glm::quat(
            glm::radians(orientations.get<glm::vec3>(items))
          );
      }
      if (display->columns.size() == 0)
      {
        auto column = display->columns.emplace(display->columns.end());
        column->column = 1.f;
        items++;
        if (offsets.valid())
          column->slot.offset = offsets.get<glm::vec3>(items);
        if (orientations.valid())
          column->slot.orientation = glm::quat(
            glm::radians(orientations.get<glm::vec3>(items))
          );
      }
    }
    if (rows.size() == 0)
    {
      auto display = rows.emplace(rows.end());
      display->row = 1.f;
      auto column = display->columns.emplace(display->columns.end());
      column->column = 1.f;
      items++;
      if (offsets.valid())
        column->slot.offset = offsets.get<glm::vec3>(items);
      if (orientations.valid())
          column->slot.orientation = glm::quat(
            glm::radians(orientations.get<glm::vec3>(items))
          );
    }

    LOG_DEBUG("Items: %u", items);
  }

  ItemDisplaySystem::ItemDisplaySystem(World *w) :
    System(w)
  {
    components->registerFactory("item_display", [&](auto id, auto table)
    {
      return std::make_shared<ItemDisplayComponent>(id, table);
    });

    subscribe("pick_item", MSGBIND(&ItemDisplaySystem::interact));
  }

  void ItemDisplaySystem::interact(const MessageSubject &, const MessageBody &body)
  {
    Message::Actor m(body);
    ActorId id = m->id();

    auto inv = components->getComponent<InventoryComponent>(id);
    if (!inv || !inv->enabled())
      return;

    auto sight = components->getComponent<SightComponent>(id);
    if (!sight || !sight->enabled())
      return;
   
    auto display = components->getComponent<ItemDisplayComponent>(sight->target);
    glm::vec3 point = sight->target_position;
    if (!display || !display->enabled())
      return;
    auto rb = components->getComponent<RigidbodyComponent>(sight->target);
    if (!rb || !rb->enabled())
      return;
    point = rb->getLocalTransform() * glm::vec4(point, 1.f);
    point.x = glm::clamp(point.x / rb->width + 0.5, 0., 1.);
    point.y = glm::clamp(point.y / rb->height + 0.5, 0., 1.);

    // Find at which row of the display the character is looking at
    unsigned int row = 0;
    float offset = display->rows[0].row;
    while(offset < point.y && row++ < display->rows.size())
    {
      offset += display->rows[row].row;
    }

    // Find at which column of the display the character is looking at
    unsigned int col = 0;
    offset = display->rows[row].columns[0].column;
    while(offset < point.x && col++ < display->rows[row].columns.size())
    {
      offset += display->rows[row].columns[col].column;
    }
    InventorySlot *display_slot = &(display->rows[row].columns[col].slot);

    auto group = inv->inventory.find("RIGHT HAND");
    if (group == inv->inventory.end())
      return;
    InventorySlot *inv_slot = &(group->second.slots[0]);

    if (inv_slot->item && !display_slot->item)
    {
      // Move the item from the hand to the display
      world->disableActor(inv_slot->item->owner);
      auto model = components->getComponent<ModelComponent>(inv_slot->item->owner);
      if (model)
      {
        model->parent = display->owner;
        model->parent_node = "body";
        model->offset = display_slot->offset;
        model->orientation = display_slot->orientation;
      }
      components->enableComponent<ModelComponent>(inv_slot->item->owner);
      components->enableComponent<AnimationComponent>(inv_slot->item->owner);

      display_slot->quantity = 1;
      display_slot->item = inv_slot->item;
      inv_slot->quantity = 0;
      inv_slot->item = ItemComponentPtr();

      Message::Item msg(id, group->first, 0, 0);
      msg->set_target(display_slot->item->owner);
      msg->set_name(inv_slot->item ? inv_slot->item->name : "");
      msg->set_description(inv_slot->item ? inv_slot->item->description : "");
      trigger("dropped_item", msg);
    }
    else if (!inv_slot->item && display_slot->item)
    {
      // Move the item from the display to the hand
      world->disableActor(display_slot->item->owner);
      auto model = components->getComponent<ModelComponent>(display_slot->item->owner);
      if (model)
      {
        model->parent = id;
        model->parent_node = group->second.node;
        model->offset = inv_slot->offset + inv_slot->orientation *
          display_slot->item->offset;
        model->orientation = inv_slot->orientation * display_slot->item->orientation;
      }
      components->enableComponent<ModelComponent>(display_slot->item->owner);
      components->enableComponent<AnimationComponent>(display_slot->item->owner);

      inv_slot->quantity = 1;
      inv_slot->item = display_slot->item;
      display_slot->quantity = 0;
      display_slot->item = ItemComponentPtr();

      Message::Item msg(id, group->first, 0, 1);
      msg->set_target(inv_slot->item->owner);
      msg->set_name(inv_slot->item->name);
      msg->set_description(inv_slot->item->name);
      trigger("picked_item", msg);

    }
  }

  void ItemDisplaySystem::update(float)
  {

  }
}
