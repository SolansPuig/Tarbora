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

#ifndef __COMPONENT_MANAGER_H_
#define __COMPONENT_MANAGER_H_

#include "System.hpp"

#define FCTBIND(T) (std::bind(T, this, std::placeholders::_1, std::placeholders::_2))
#define ENBIND(T) (std::bind(T, this, std::placeholders::_1))

/**
 * \file
 * \brief Classes \ref Tarbora::Component, \ref Tarbora::ComponentIterator and
 *        \ref Tarbora::ComponentManager
 */

namespace Tarbora {

  //! A virtual component for the \ref Entity_Component_System.
  /*!
   * All components should inherit from this. When creating a new component, keep in
   * mind that it should only contain configuration code and data. Update functions,
   * event parsing and similar should be done by a \ref Tarbora::System.
   */
  class Component {
  public:
    //! Constructor for a component.
    /*!
     * Configuration code goes here.
     *
     * Set the protected property \c error_ to false if there is an error during the
     * configuration. This will disable the component. When this happens, also
     * \ref Tarbora::Logger::log "log" the cause of the error.
     *
     * \param id The id of the actor that owns that component.
     * \param table A Lua Table containing the configuration data for the component.
     *
     * See also \ref Tarbora::ComponentManager.
     */
    Component (const ActorId &id, const LuaTable &table);

    //! Enable the component. This won't work if it has errors.
    void enable() { enabled_ = !error_; }
    //! Disable the component.
    /*!
     * Disabling a component is like destroying it, but keeping all its data, in case
     * it is enabled again later. Don't use this function if you just want to destroy
     * it forever.
     */
    void disable() { enabled_ = false; }
    //! Check if the component is enabled.
    bool enabled() const { return enabled_; }

    //! Check if the component found any errors during initialization.
    bool error() const { return error_; }

    virtual ComponentId getType() const = 0;

    //! The id of the entity that owns that component.
    ActorId owner;
    bool enabling{false};

  protected:
    bool enabled_{false};
    bool start_enabled_{true};
    bool error_{false};
  };

  typedef std::shared_ptr<Component> ComponentPtr;
  typedef std::pair<const ComponentId, ComponentPtr> ComponentPair;
  typedef std::function<ComponentPtr(const ActorId&, const LuaTable&)> ComponentFactory;
  typedef std::function<bool(ComponentPtr)> ComponentOnEnable;
  typedef std::unordered_map<ComponentId, ComponentPtr> ComponentMap;
  typedef std::list<std::weak_ptr<Component>> ComponentList;

  //! Just an iterator for \ref Tarbora::Component lists.
  class ComponentIterator {
    friend class ComponentManager;
  public:
    auto begin() { return begin_; }
    auto end() const { return end_; }

    operator bool() const { return valid_; }

  private:
    typename ComponentList::iterator begin_;
    typename ComponentList::iterator end_;
    bool valid_{false};
  };

  //! Just an iterator for \ref Tarbora::Component maps.
  class ComponentMapIterator {
    friend class ComponentManager;
  public:
    auto begin() { return begin_; }
    auto end() const { return end_; }

    operator bool() const { return valid_; }

  private:
    ComponentMap::iterator begin_;
    ComponentMap::iterator end_;
    bool valid_{false};
  };

  //! The class that stores all the components. See \ref Entity_Component_System.
  class ComponentManager {
  public:
    //! Create a new component of type \c component for an actor.
    /*!
     * If this actor already has a component of that type, this won't create a new
     * one, just return a pointer to the old.
     *
     * \param id The id of the actor that owns that component.
     * \param component The id of the component type.
     * \param table A Lua Table containing the configuration data for the component.
     * \return A pointer to the newly created component.
     */
    ComponentPtr createComponent(
      const ActorId &id,
      const ComponentId &component,
      const LuaTable &table
    );

    //! Get a component of type T from an actor.
    /*!
     * \param id The id of the actor that owns the component.
     * \return A pointer to the requested component, or \c nullptr if not found.
     */
    template <class T>
    std::shared_ptr<T> getComponent(const ActorId &id);

    //! Delete the component of type T from an actor, if exists.
    template <class T>
    void deleteComponent(const ActorId &id);

    //! Get a \ref Tarbora::ComponentIterator to all of the components of type T.
    template <class T>
    ComponentIterator getComponents();
    //! Get a \ref Tarbora::ComponentIterator to all of the components of an entity.
    ComponentMapIterator getEntityComponents(const ActorId &id);

    //! Delete all the components of an actor.
    void deleteEntityComponents(const ActorId &id);

    //! Register a new component creator function
    /*!
     * \param id The id of the component type.
     * \param factory The factory function.
     */
    void registerFactory(const ComponentId &id, ComponentFactory factory);

    //! Enable the component of type T of an actor.
    template <class T>
    ComponentPtr enableComponent(const ActorId &id);

    //! Disable the component of type T of an actor.
    template <class T>
    ComponentPtr disableComponent(const ActorId &id);

    //! Require the component of type T to be enabled.
    template <class T>
    std::shared_ptr<T> require(const ActorId &id);

    //! Enable all the components of an actor.
    bool enableEntityComponents(const ActorId &id);
    //! Disable all the components of an actor.
    bool disableEntityComponents(const ActorId &id);


    //! Register an enable callback.
    /*!
     * \param id The id of the component type.
     * \param callback The callback function that will be called before enabling
     * that component.
     */
    void onEnable(const ComponentId &id, ComponentOnEnable callback);
    //! Register a disable callback.
    /*!
     * \param id The id of the component type.
     * \param callback The callback function that will be called before disabling
     * that component.
     */
    void onDisable(const ComponentId &id, ComponentOnEnable callback);

    /*! Delete entities marked for deletion. Not thread safe, cannot be called during a
     * system update.
     */
    void clean();

  private:
    bool enableComponent(std::shared_ptr<Component> comp);
    bool disableComponent(std::shared_ptr<Component> comp);

    std::unordered_map<ComponentId, ComponentList>  components_;
    std::unordered_map<ActorId, ComponentMap> entities_;
    std::unordered_map<ComponentId, ComponentFactory> component_factory_;
    std::unordered_map<ComponentId, ComponentOnEnable> component_enablers_;
    std::unordered_map<ComponentId, ComponentOnEnable> component_disablers_;
    std::list<ActorId> entities_to_delete_;
  };


  // Implementation of ComponentManager

  template <class T>
  std::shared_ptr<T> ComponentManager::getComponent(const ActorId &id)
  {
    auto entity = entities_.find(id);
    if (entity != entities_.end())
    {
      auto component = entity->second.find(T::getStaticType());
      if (component != entity->second.end())
      {
        return std::static_pointer_cast<T>(component->second);
      }
    }

    return std::shared_ptr<T>();
  }

  template <class T>
  void ComponentManager::deleteComponent(const ActorId &id)
  {
    // Delete from entity
    auto entity = entities_.find(id);
    if (entity != entities_.end())
    {
      entity->second.erase(T::getStaticType());
    }
  }

  template <class T>
  ComponentIterator ComponentManager::getComponents()
  {
    ComponentIterator iter;

    auto list = components_.find(T::getStaticType());
    if (list != components_.end())
    {
      iter.begin_ = list->second.begin();
      iter.end_ = list->second.end();
      iter.valid_ = true;
    }

    return iter;
  }

  template <class T>
  ComponentPtr ComponentManager::enableComponent(const ActorId &id)
  {
    std::shared_ptr<T> comp = getComponent<T>(id);
    if(comp && enableComponent(comp))
      return comp;
    return ComponentPtr();
  }

  template <class T>
  ComponentPtr ComponentManager::disableComponent(const ActorId &id)
  {
    std::shared_ptr<T> comp = getComponent<T>(id);
    if (comp && disableComponent(comp))
      return comp;
    return ComponentPtr();
  }

  template <class T>
  std::shared_ptr<T> ComponentManager::require(const ActorId &id)
  {
    return std::static_pointer_cast<T>(enableComponent<T>(id));
  }
}

#endif // __COMPONENTMANAGER_H_
