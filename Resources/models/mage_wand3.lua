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
      scale = 1.00,
      size = {5.00, 160.00, 5.00},
      shape = "cube.mesh",
      uv_map = {0, 0},
      color_primary = {156, 127, 98},
      color_secondary = {254, 255, 255},
      nodes = {
        {
          name = "ring",
          scale = 1.00,
          position = {0.00, 81.00, 0.00},
          size = {6.00, 3.00, 6.00},
          shape = "cube.mesh",
          uv_map = {0, 0},
          color_primary = {238, 200, 40},
          nodes = {
            {
              name = "magic_ball",
              scale = 1.00,
              position = {0.00, 11.00, 0.00},
              rotation = {-39.00, 0.00, -156.00},
              size = {10.00, 10.00, 10.00},
              shape = "cube.mesh",
              uv_map = {20, 30},
              color_primary = {255, 235, 0},
              animated = true,
            },
          }
        },
      }
    },
  }
}
