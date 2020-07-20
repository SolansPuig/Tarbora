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
      size = {5.00, 156.00, 5.00},
      shape = "cube.mesh",
      uv_map = {0, 0},
      color_primary = {61, 61, 61},
      color_secondary = {254, 255, 255},
      nodes = {
        {
          name = "magic_ball",
          scale = 1.00,
          position = {0.00, 97.00, 0.00},
          rotation = {58.00, 59.00, 43.00},
          size = {7.00, 7.00, 7.00},
          shape = "cube.mesh",
          uv_map = {20, 30},
          texture_size = {10.00, 10.00, 10.00},
          mesh_size = {10.00, 10.00, 10.00},
          color_primary = {243, 55, 0},
          animated = true,
        },
        {
          name = "wand",
          scale = 1.00,
          origin = {0.00, -0.50, 0.50},
          position = {0.00, 85.00, 0.00},
          rotation = {20.00, -0.00, 0.00},
          size = {5.00, 25.00, 5.00},
          shape = "cube.mesh",
          uv_map = {20, 0},
          color_primary = {61, 61, 61},
          nodes = {
            {
              name = "wand2",
              scale = 1.00,
              origin = {0.00, -0.50, 0.50},
              position = {0.00, 25.00, 0.00},
              rotation = {-40.00, -0.00, 0.00},
              size = {5.00, 25.00, 5.00},
              shape = "cube.mesh",
              uv_map = {20, 0},
              color_primary = {61, 61, 61},
            },
          }
        },
        {
          name = "wand3",
          scale = 1.00,
          origin = {-0.00, -0.50, -0.50},
          position = {0.00, 85.00, 0.00},
          rotation = {-20.00, -0.00, 0.00},
          size = {5.00, 25.00, 5.00},
          shape = "cube.mesh",
          uv_map = {20, 0},
          color_primary = {61, 61, 61},
          nodes = {
            {
              name = "wand4",
              scale = 1.00,
              origin = {0.00, -0.50, -0.50},
              position = {0.00, 25.00, 0.00},
              rotation = {40.00, -0.00, 0.00},
              size = {5.00, 25.00, 5.00},
              shape = "cube.mesh",
              uv_map = {20, 0},
              color_primary = {61, 61, 61},
            },
          }
        },
        {
          name = "wand5",
          scale = 1.00,
          origin = {-0.50, -0.50, -0.50},
          position = {0.00, 85.00, 0.00},
          rotation = {0.00, -0.00, 20.00},
          size = {5.00, 25.00, 5.00},
          shape = "cube.mesh",
          uv_map = {20, 0},
          color_primary = {61, 61, 61},
          nodes = {
            {
              name = "wand6",
              scale = 1.00,
              origin = {-0.50, -0.50, 0.00},
              position = {0.00, 25.00, 0.00},
              rotation = {0.00, -0.00, -40.00},
              size = {5.00, 25.00, 5.00},
              shape = "cube.mesh",
              uv_map = {20, 0},
              color_primary = {61, 61, 61},
            },
          }
        },
        {
          name = "wand7",
          scale = 1.00,
          origin = {0.50, -0.50, 0.00},
          position = {0.00, 85.00, 0.00},
          rotation = {0.00, -0.00, -20.00},
          size = {5.00, 25.00, 5.00},
          shape = "cube.mesh",
          uv_map = {20, 0},
          color_primary = {61, 61, 61},
          nodes = {
            {
              name = "wand8",
              scale = 1.00,
              origin = {0.50, -0.50, 0.00},
              position = {0.00, 25.00, 0.00},
              rotation = {0.00, -0.00, 40.00},
              size = {5.00, 25.00, 5.00},
              shape = "cube.mesh",
              uv_map = {20, 0},
              color_primary = {61, 61, 61},
            },
          }
        },
      }
    },
  }
}