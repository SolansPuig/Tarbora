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

#ifndef __MODULECOMPONENT_H_
#define __MODULECOMPONENT_H_

#include "Module.hpp"

/**
 * @file
 * @brief Class @ref Tarbora::ModuleComponent
*/

namespace Tarbora {
  //! A part of a module with a defined function.
  class ModuleComponent
  {
  public:
    //! Constructor
    /*!
      \param m A pointer to the module that owns this component.
    */
    ModuleComponent(Module *m) : module_(m) {}
    ~ModuleComponent();

    //! Perform the logic and the computations of the component.
    /*!
      When this function is overrided in a child component, it will be called inside of
      Module::update.
      \param delta_time The elapsed time, in seconds, since the last iteration.
    */
    virtual void update(float delta_time) { UNUSED(delta_time); };

    //! Render the data of the module.
    /*!
      When this function is overrided in a child component, it will be called inside of
      Module::draw.
    */
    virtual void draw() {};

    inline void send(
      const ClientId &to, const MessageSubject &s, const MessageBody &b
    ) const
    {
      module_->getMessageManager()->send(to, s, b);
    }

    inline void trigger(const MessageSubject &s, const MessageBody &b) const
    {
      module_->getMessageManager()->trigger(s, b);
    }

    inline void triggerLocal(const MessageSubject &s, const MessageBody &b) const
    {
      module_->getMessageManager()->triggerLocal(s, b);
    }

  protected:
    // Only module components should be able to subscribe and descubscribe
    void subscribe(const MessageSubject &s, MessageFn callback);
    void desubscribe(const MessageSubject &s);

    Module *module_;

  private:
    std::unordered_map<std::string, SubscriptorId> subscribed_messages_;
  };
}

#endif // __MODULECOMPONENT_H_
