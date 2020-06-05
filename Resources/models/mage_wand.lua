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

scale = 1
nodes = {
  {
    name = "body",
    scale = 1.00,
    size = {5.00, 160.00, 5.00},
    shape = "cube.mesh",
    uv_map = {0, 0},
    color_primary = {233, 233, 233},
    color_secondary = {254, 255, 255},
    nodes = {
      {
        name = "wand",
        scale = 1.00,
        origin = {0.00, -0.50, -0.50},
        position = {0.00, 92.50, 0.00},
        rotation = {40.00, -0.00, 0.00},
        size = {5.00, 25.00, 5.00},
        shape = "cube.mesh",
        uv_map = {20, 0},
        color_primary = {233, 233, 233},
        nodes = {
          {
            name = "wand2",
            scale = 1.00,
            origin = {0.00, -0.50, 0.50},
            position = {0.00, 25.00, 0.00},
            rotation = {-90.00, -0.00, 0.00},
            size = {5.00, 25.00, 5.00},
            shape = "cube.mesh",
            uv_map = {20, 0},
            color_primary = {233, 233, 233},
            nodes = {
              {
                name = "wand3",
                scale = 1.00,
                origin = {0.00, -0.50, 0.00},
                position = {0.00, 17.50, -13.00},
                rotation = {90.00, -0.00, 0.00},
                size = {5.00, 15.00, 5.00},
                shape = "cube.mesh",
                uv_map = {20, 0},
                color_primary = {233, 233, 233},
                nodes = {
                  {
                    name = "magic_ball",
                    scale = 1.00,
                    position = {0.00, -9.00, 10.00},
                    rotation = {0.00, -0.00, -156.00},
                    size = {10.00, 10.00, 10.00},
                    shape = "cube.mesh",
                    uv_map = {20, 30},
                    color_primary = {0, 255, 180},
                    animated = true,
                  },
                }
              },
            }
          },
        }
      },
    }
  },
}
