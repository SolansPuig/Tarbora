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

#ifndef __SCENEMANAGER_H_
#define __SCENEMANAGER_H_

#include "../../../Framework/Module/Module.hpp"
#include "../../Framework/Global.hpp"

namespace Tarbora {
  class SceneManager : public Module
  {
  public:
    SceneManager() : Module(2) {}
    ~SceneManager() {}

    void load(const std::string &level);
    virtual void update(float) override {}

    void loadActor(LuaTable table);
  };
}

#endif // __SCENEMANAGER_H_
