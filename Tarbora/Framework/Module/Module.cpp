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

#include "Module.hpp"

namespace Tarbora {
  Module::Module(const ClientId &id) :
    AbstractModule()
  {
    message_manager_ = std::shared_ptr<MessageManager>(new MessageManager(id));
  }

  Module::~Module() {}

  void Module::run(const std::string &name)
  {
    ZoneScoped;
    message_manager_->setDebugName(name + " Message Client");

    time_ = std::chrono::system_clock::now();
    while(AbstractModule::run_)
    {
      const auto current_time = std::chrono::system_clock::now();
      std::chrono::duration<float> duration = current_time - time_;
      const float delta_time = duration.count();

      if (delta_time >= 1./max_fps_)
      {
        time_ = current_time;

        // Inform the profiler that the frame changed
        if (!using_thread_) FrameMark;
        FrameMarkNamed(name.c_str());

        message_manager_->readMessages();
        getInput();
        update(delta_time);
        draw();
      }
    }
  }
}
