--[===================================================================[
  * Copyright (C) 2020 Roger Solans Puig
  * Email: roger@solanspuig.cat
  *
  * This file is part of Tarbora. You can obtain a copy at
  * https://github.com/SolansPuig/Tarbora
  *
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at https://mozilla.org/MPL/2.0/.
--]===================================================================]

components = {
  type = {"ground"},
  model = {
    render_pass = 1,
    model = "forest_ground.lua",
    material = "grass.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {34, 1, 34},
    density = 0,
    friction = 1,
    restitution = 0.3
  }
}
