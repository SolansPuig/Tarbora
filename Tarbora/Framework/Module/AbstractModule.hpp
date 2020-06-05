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

#ifndef __ABSTRACTMODULE_H_
#define __ABSTRACTMODULE_H_

#include "../CommonHeaders.hpp"
#include "../Global.hpp"

/** @dir Module
 * @brief The main parts that compose the engine
*/
/**
 * @file
 * @brief Class @ref Tarbora::AbstractModule
*/

namespace Tarbora {
  //! An abstract module that allows all modules to run as threads.
  class AbstractModule
  {
  public:
    AbstractModule() : using_thread_(false){}

    ~AbstractModule();

    //! Start this module's loop.
    /*!
      All modules should stop when AbstractModule::run_ is set to false.
      \param name The name of the module, only used for the profiler.
    */
    virtual void run(const std::string &name="Unamed_Module") = 0;

    //! Start this module's loop in a new thread.
    /*!
      This function calls AbstractModule::run in a new thread.
      \param name The name of the module, only used for the profiler.
    */
    void runThread(const std::string &name="Unamed_Module");

    //! Stop all the modules.
    void close();

  protected:
    static bool run_;
    bool using_thread_;

  private:
    std::thread thread_;
  };
}

#endif // __ABSTRACTMODULE_H_
