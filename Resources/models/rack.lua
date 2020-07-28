--[===================================================================[
  * Copyright (C) 2020 Roger Solans Puig
  * Email: roger@solanspuig.cat
  * 
  * This file is part of Tarbora. You can download the project at
  * https://github.com/SolansPuig/Tarbora 
  * 
  * This Source Code Form is subject to the terms of the Mozilla Public
  * License, v. 2.0. If a copy of the MPL was not distributed with this
  * file, You can obtain one at https://mozilla.org/MPL/2.0/. 
--]===================================================================]

actor = {
  scale = 1.00,
  nodes = {
    {
      name = "body",
      rotation = {180.00, -0.00, 180.00},
      shape = "cube.mesh",
      uv_map = {200.00, 100.00},
      color_primary = {148.00, 130.00, 82.00},
      nodes = {
        {
          name = "support3",
          position = {0.00, 12.00, -8.00},
          rotation = {0.00, -0.00, 90.00},
          size = {5.00, 150.00, 5.00},
          shape = "cube.mesh",
          uv_map = {132.00, 0.00},
          color_primary = {148.00, 130.00, 82.00},
        },
        {
          name = "support2",
          position = {0.00, 45.00, -5.00},
          rotation = {0.00, -0.00, 90.00},
          size = {5.00, 150.00, 5.00},
          shape = "cube.mesh",
          uv_map = {132.00, 0.00},
          color_primary = {148.00, 130.00, 82.00},
        },
        {
          name = "support1",
          position = {0.00, -30.00, -25.00},
          rotation = {0.00, -0.00, 90.00},
          size = {5.00, 150.00, 5.00},
          shape = "cube.mesh",
          uv_map = {132.00, 0.00},
          color_primary = {148.00, 130.00, 82.00},
        },
        {
          name = "leg2",
          position = {63.00, 5.00, -21.00},
          rotation = {6.00, -0.00, 0.00},
          size = {20.00, 111.00, 20.00},
          origin = {0.00, -0.50, 0.00},
          shape = "cube.mesh",
          uv_map = {0.00, 0.00},
          color_primary = {148.00, 130.00, 82.00},
          color_emissive = {32.00, 255.00, 0.00},
          nodes = {
            {
              name = "leg23",
              position = {0.00, -24.00, 21.00},
              rotation = {-96.00, -0.00, 0.00},
              size = {8.00, 77.00, 18.00},
              shape = "cube.mesh",
              uv_map = {80.00, 0.00},
              color_primary = {148.00, 130.00, 82.00},
            },
            {
              name = "leg22",
              position = {0.00, 4.00, 60.00},
              rotation = {-31.00, -0.00, 0.00},
              size = {15.00, 111.00, 15.00},
              origin = {0.00, -0.50, 0.00},
              shape = "cube.mesh",
              uv_map = {0.00, 0.00},
              texture_size = {20.00, 111.00, 20.00},
              mesh_size = {20.00, 111.00, 20.00},
              color_primary = {148.00, 130.00, 82.00},
              color_emissive = {32.00, 255.00, 0.00},
            },
          },
        },
        {
          name = "leg1",
          position = {-63.00, 5.00, -21.00},
          rotation = {6.00, -0.00, 0.00},
          size = {20.00, 111.00, 20.00},
          origin = {0.00, -0.50, 0.00},
          shape = "cube.mesh",
          uv_map = {0.00, 0.00},
          color_primary = {148.00, 130.00, 82.00},
          color_emissive = {32.00, 255.00, 0.00},
          nodes = {
            {
              name = "leg13",
              position = {0.00, -24.00, 21.00},
              rotation = {-96.00, -0.00, 0.00},
              size = {8.00, 77.00, 18.00},
              shape = "cube.mesh",
              uv_map = {80.00, 0.00},
              color_primary = {148.00, 130.00, 82.00},
            },
            {
              name = "leg12",
              position = {0.00, 4.00, 60.00},
              rotation = {-31.00, -0.00, 0.00},
              size = {15.00, 111.00, 15.00},
              origin = {0.00, -0.50, 0.00},
              shape = "cube.mesh",
              uv_map = {0.00, 0.00},
              texture_size = {20.00, 111.00, 20.00},
              mesh_size = {20.00, 111.00, 20.00},
              color_primary = {148.00, 130.00, 82.00},
              color_emissive = {32.00, 255.00, 0.00},
            },
          },
        },
      },
    },
  },
}
