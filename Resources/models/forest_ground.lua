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

actor = {
  scale = 1,
  colors = {
    ["Grass"] = {181, 255, 101}
  },
  nodes = {
    {
      name = "body",
      shape = "cube.mesh",
      origin = {0, 0, 0},
      position = {0, 0, 0},
      rotation = {0, 0, 0},
      size = {3400, 100, 3400},
      texture_size = {100, 0, 100},
      uv_map = {0, 0},
      color_primary = "Grass"
    }
  }
}
