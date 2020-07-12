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
      size = {30.00, 6.00, 10.00},
      shape = "cube.mesh",
      uv_map = {0, 0},
      color_primary = {255, 254, 254},
      nodes = {
        {
          name = "barrel",
          scale = 1.00,
          position = {9.00, -6.00, 0.00},
          rotation = {0.00, -0.00, -4.00},
          size = {12.00, 7.00, 8.00},
          shape = "cube.mesh",
          uv_map = {0, 0},
          color_primary = {254, 255, 254},
          nodes = {
            {
              name = "thing",
              scale = 1.00,
              position = {-10.70, 2.70, 0.00},
              rotation = {0.00, -0.00, -26.00},
              size = {14.00, 7.00, 8.00},
              shape = "cube.mesh",
              uv_map = {0, 0},
              color_primary = {254, 255, 254},
            },
          }
        },
        {
          name = "handle",
          scale = 1.00,
          position = {-14.00, -5.00, 0.00},
          rotation = {0.00, -0.00, 60.00},
          size = {11.00, 7.00, 5.00},
          shape = "cube.mesh",
          uv_map = {0, 0},
        },
        {
          name = "point",
          scale = 1.00,
          position = {16.00, 0.00, 0.00},
          size = {2.00, 4.00, 4.00},
          shape = "cube.mesh",
          uv_map = {0, 0},
          color_primary = {254, 254, 255},
        },
        {
          name = "trigger",
          scale = 1.00,
          position = {-6.00, -5.00, 0.00},
          size = {10.00, 4.00, 2.00},
          shape = "cube.mesh",
          uv_map = {0, 0},
          color_primary = {255, 254, 254},
        },
      }
    },
  }
}
