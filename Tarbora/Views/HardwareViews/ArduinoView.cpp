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

#include "ArduinoView.hpp"
#include "../../Messages/BasicMessages.hpp"

namespace Tarbora {
  ArduinoView::ArduinoView()
    : Module(22)
  {
    LOG_DEBUG("Creating arduino view...");

    arduino_.open("/dev/ttyACM0");

    LOG_DEBUG("Successfully created arduino game view");
  }

  ArduinoView::~ArduinoView()
  {
    LOG_DEBUG("Destroying arduino game view...");
    arduino_.close();
    LOG_DEBUG("Successfully destroyed arduino game view");
  }

  void ArduinoView::update(float delta_time)
  {
    std::string str;
    std::getline(arduino_, str);
    std::istringstream line(str);

    std::string number;
    std::getline(line, number, ',');
    value.z += std::strtof(number.c_str(), nullptr) * delta_time;

    std::getline(line, number, ',');
    value.x += std::strtof(number.c_str(), nullptr) * delta_time;

    std::getline(line, number, ',');
    value.x += std::strtof(number.c_str(), nullptr) * delta_time;

    getMessageManager()->trigger(
      "move",
      Message::ApplyPhysics("plat_3", -value / 10.f)
    );

  }
}
