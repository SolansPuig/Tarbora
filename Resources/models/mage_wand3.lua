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
  colors = {
    ["Wand"] = {156, 127, 98},
    ["Magic Stone"] = {255, 235, 0},
    ["Ring"] = {238, 200, 40}
  },
  nodes = {
    {
      name = "body",
      size = {5.00, 160.00, 5.00},
      shape = "cube.mesh",
      uv_map = {0.00, 0.00},
      color_primary = "Wand",
      nodes = {
        {
          name = "ring",
          position = {0.00, 81.00, 0.00},
          size = {6.00, 3.00, 6.00},
          shape = "cube.mesh",
          uv_map = {0.00, 0.00},
          color_primary = "Ring",
          nodes = {
            {
              name = "magic_ball",
              animated = true,
              position = {0.00, 11.00, 0.00},
              rotation = {-39.00, 0.00, -156.00},
              size = {10.00, 10.00, 10.00},
              shape = "cube.mesh",
              uv_map = {20.00, 30.00},
              color_primary = "Magic Stone",
              nodes = {
                {
                  name = "light_2",
                  type = "light",
                  shape = "sphere.mesh",
                  shader = "point_light.shader.lua",
                  ambient = {11.84, 10.50, 0.00},
                  diffuse = {255.00, 213.04, 0.00},
                  specular = {41.96, 37.22, 0.00},
                  direction = {1.00, 1.00, 1.00},
                  intensity = 0.20,
                  attenuation = {1.00, 1.00},
                },
              },
            },
          },
        },
      },
    },
  },
}
