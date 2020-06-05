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

#include "AbstractModule.hpp"

namespace Tarbora {
  AbstractModule::~AbstractModule()
  {
    if (using_thread_) thread_.join();
  }

  void AbstractModule::runThread(const std::string &name)
  {
    using_thread_ = true;
    thread_ = std::thread(&AbstractModule::run, this, name);
    tracy::SetThreadName(thread_, name.c_str());
  }

  void AbstractModule::close()
  {
    AbstractModule::run_ = false;
  }

  bool AbstractModule::run_ = true;
}
