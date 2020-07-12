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
  type = {"tree"},
  model = {
    render_pass = 1,
    model = "birch.lua",
    material = "birch.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {0.75, 2.25, 0.75},
    density = 0,
    friction = 1,
    restitution = 0.3
  }
}
