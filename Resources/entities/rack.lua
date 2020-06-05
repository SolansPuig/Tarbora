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
  type = {"rack"},
  model = {
    render_pass = 1,
    model = "rack.lua",
    material = "rack.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {1.2, 1, 0.5},
    density = 0
  },
  item_display = {
    items = {
      { row = 1/1, columns = {1/3, 1/3, 1/3} },
    },
    offsets = { {1/3, 0, -0.15}, {0, 0, -0.15}, {-1/3, 0, -0.15} },
    orientations = { {5, 0, 0}, {5, 0, 0}, {5, 0, 0} }
  }
}
