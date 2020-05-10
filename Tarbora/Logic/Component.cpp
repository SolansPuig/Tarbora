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

#include "Component.hpp"

namespace Tarbora {
  Component::Component(const ActorId &id, const LuaTable &table) :
    owner(id)
  {
    start_enabled_ = table.get<bool>("start_enabled", true, true);
  }

  ComponentPtr ComponentManager::createComponent(
    const ActorId &id,
    const ComponentId &component,
    const LuaTable &table
  )
  {
    ComponentPtr result;

    // Check if entity exists
    auto entity_components = entities_.find(id);
    if (entity_components != entities_.end())
    {
      // Check if already has a component of that type
      auto comp = entity_components->second.find(component);
      if (comp != entity_components->second.end())
      {
        LOG_WARN(
          "Component Manager: Trying to create %s for entity %s that already has that "
          "component", component.c_str(), id.c_str()
        );

        return comp->second;
      }
      else
      {
        // Add component to entity
        auto factory = component_factory_.find(component);
        if (factory == component_factory_.end())
        {
          LOG_WARN(
            "Component Manager: Could not find a component named %s for"
            " entity %s", component.c_str(), id.c_str());
          return result;
        }
        result = factory->second(id, table);
        entity_components->second.emplace(component, result);
      }
    }
    else
    {
      // Create entity and add the component
      auto factory = component_factory_.find(component);
      if (factory == component_factory_.end())
      {
        LOG_WARN(
          "Component Manager: Could not find a component named %s for entity %s",
          component.c_str(), id.c_str());
        return result;
      }
      result = factory->second(id, table);
      ComponentMap components;
      components.emplace(component, result);
      entities_.emplace(id, components);
    }

    // Add component to components list
    auto list = components_.find(component);
    if (list != components_.end())
    {
      list->second.emplace_back(result);
    }
    else
    {
      ComponentList components;
      components.emplace_back(result);
      components_.emplace(component, components);
    }

    return result;
  }

  void ComponentManager::registerFactory(
    const ComponentId &id,
    ComponentFactory factory
  )
  {
    component_factory_.emplace(id, factory);
  }

  ComponentMapIterator ComponentManager::getEntityComponents(const ActorId &id)
  {
    ComponentMapIterator iter;

    auto entity = entities_.find(id);
    if (entity != entities_.end())
    {
      iter.begin_ = entity->second.begin();
      iter.end_ = entity->second.end();
      iter.valid_ = true;
    }

    return iter;
  }

  void ComponentManager::deleteEntityComponents(const ActorId &id)
  {
    entities_to_delete_.push_back(id);
  }

  bool ComponentManager::enableEntityComponents(const ActorId &id)
  {
    bool ok = true;
    auto entity = entities_.find(id);
    if (entity == entities_.end())
      return false;

    for (auto comp : entity->second)
    {
      if (!enableComponent(comp.second))
        ok = false;
    }

    return ok;
  }

  bool ComponentManager::disableEntityComponents(const ActorId &id)
  {
    bool ok = true;
    auto entity = entities_.find(id);
    if (entity == entities_.end())
      return false;

    for (auto comp : entity->second)
    {
      if (!disableComponent(comp.second))
        ok = false;
    }

    return ok;
  }

  bool ComponentManager::enableComponent(std::shared_ptr<Component> comp)
  {
    if (comp->enabled())
      return true;

    if (!comp->error())
    {
      auto enabler = component_enablers_.find(comp->getType());
      if (enabler == component_enablers_.end() || enabler->second(comp))
      {
        comp->enable();
        return true;
      }
    }

    return false;
  }

  bool ComponentManager::disableComponent(std::shared_ptr<Component> comp)
  {
    if (!comp->enabled())
      return true;

    auto disabler = component_disablers_.find(comp->getType());
    if (disabler == component_disablers_.end() || disabler->second(comp))
    {
      comp->disable();
      return true;
    }

    return false;
  }

  void ComponentManager::onEnable(const ComponentId &id, ComponentOnEnable callback)
  {
    component_enablers_.emplace(id, callback);
  }

  void ComponentManager::onDisable(const ComponentId &id, ComponentOnEnable callback)
  {
    component_disablers_.emplace(id, callback);
  }

  void ComponentManager::clean()
  {
    for (auto entity : entities_to_delete_)
    {
      entities_.erase(entity);
    }
  }
}
