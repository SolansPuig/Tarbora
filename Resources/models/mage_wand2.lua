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
      size = {5.00, 156.00, 5.00},
      shape = "cube.mesh",
      uv_map = {0.00, 0.00},
      color_primary = {61.00, 61.00, 61.00},
      color_secondary = {254.00, 255.00, 255.00},
      nodes = {
        {
          name = "magic_ball",
          animated = true,
          position = {0.00, 97.00, 0.00},
          rotation = {58.00, 59.00, 43.00},
          size = {7.00, 7.00, 7.00},
          shape = "cube.mesh",
          uv_map = {20.00, 30.00},
          texture_size = {10.00, 10.00, 10.00},
          mesh_size = {10.00, 10.00, 10.00},
          color_primary = {243.00, 55.00, 0.00},
          nodes = {
            {
              name = "light_4",
              type = "light",
              shape = "sphere.mesh",
              shader = "point_light.shader.lua",
              ambient = {25.82, 0.00, 0.00},
              diffuse = {255.00, 12.91, 0.00},
              specular = {62.41, 0.00, 0.00},
              direction = {1.00, 1.00, 1.00},
              intensity = 0.40,
              attenuation = {1.00, 1.00},
            },
          },
        },
        {
          name = "wand5",
          position = {0.00, 85.00, 0.00},
          rotation = {0.00, -0.00, 20.00},
          size = {5.00, 25.00, 5.00},
          origin = {-0.50, -0.50, -0.50},
          shape = "cube.mesh",
          uv_map = {20.00, 0.00},
          color_primary = {61.00, 61.00, 61.00},
          nodes = {
            {
              name = "wand6",
              position = {0.00, 25.00, 0.00},
              rotation = {0.00, -0.00, -40.00},
              size = {5.00, 25.00, 5.00},
              origin = {-0.50, -0.50, 0.00},
              shape = "cube.mesh",
              uv_map = {20.00, 0.00},
              color_primary = {61.00, 61.00, 61.00},
            },
          },
        },
        {
          name = "wand",
          position = {0.00, 85.00, 0.00},
          rotation = {20.00, -0.00, 0.00},
          size = {5.00, 25.00, 5.00},
          origin = {0.00, -0.50, 0.50},
          shape = "cube.mesh",
          uv_map = {20.00, 0.00},
          color_primary = {61.00, 61.00, 61.00},
          nodes = {
            {
              name = "wand2",
              position = {0.00, 25.00, 0.00},
              rotation = {-40.00, -0.00, 0.00},
              size = {5.00, 25.00, 5.00},
              origin = {0.00, -0.50, 0.50},
              shape = "cube.mesh",
              uv_map = {20.00, 0.00},
              color_primary = {61.00, 61.00, 61.00},
            },
          },
        },
        {
          name = "wand3",
          position = {0.00, 85.00, 0.00},
          rotation = {-20.00, -0.00, 0.00},
          size = {5.00, 25.00, 5.00},
          origin = {-0.00, -0.50, -0.50},
          shape = "cube.mesh",
          uv_map = {20.00, 0.00},
          color_primary = {61.00, 61.00, 61.00},
          nodes = {
            {
              name = "wand4",
              position = {0.00, 25.00, 0.00},
              rotation = {40.00, -0.00, 0.00},
              size = {5.00, 25.00, 5.00},
              origin = {0.00, -0.50, -0.50},
              shape = "cube.mesh",
              uv_map = {20.00, 0.00},
              color_primary = {61.00, 61.00, 61.00},
            },
          },
        },
        {
          name = "wand7",
          position = {0.00, 85.00, 0.00},
          rotation = {0.00, -0.00, -20.00},
          size = {5.00, 25.00, 5.00},
          origin = {0.50, -0.50, 0.00},
          shape = "cube.mesh",
          uv_map = {20.00, 0.00},
          color_primary = {61.00, 61.00, 61.00},
          nodes = {
            {
              name = "wand8",
              position = {0.00, 25.00, 0.00},
              rotation = {0.00, -0.00, 40.00},
              size = {5.00, 25.00, 5.00},
              origin = {0.50, -0.50, 0.00},
              shape = "cube.mesh",
              uv_map = {20.00, 0.00},
              color_primary = {61.00, 61.00, 61.00},
            },
          },
        },
      },
    },
  },
}
