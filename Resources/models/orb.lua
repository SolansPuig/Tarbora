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
  nodes = {
    {
      name = "body",
      shape = "cube.mesh",
      origin = {0, 0, 0},
      position = {0, 0, 0},
      rotation = function() return {
          math.random(-45, 45), math.random(-45, 45), math.random(-45, 45)
      } end,
      size = {30, 30, 30},
      mesh_size = {10, 10, 10},
      texture_size = {10, 10, 10},
      color_primary = {255, 235, 0},
      uv_map = {20, 30},
      nodes = {
        {
          name = "round",
          shape = "cube.mesh",
          rotation = {45, 45, 0},
          size = {30, 30, 30},
          mesh_size = {10, 10, 10},
          texture_size = {10, 10, 10},
          color_primary = {255, 235, 0},
          uv_map = {20, 30}
        }
      }
    }
  }
}
