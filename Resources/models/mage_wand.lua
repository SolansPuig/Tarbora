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
      size = {5.00, 160.00, 5.00},
      shape = "cube.mesh",
      uv_map = {0.00, 0.00},
      color_primary = {233.00, 233.00, 233.00},
      color_secondary = {254.00, 255.00, 255.00},
      nodes = {
        {
          name = "wand",
          position = {0.00, 92.50, 0.00},
          rotation = {40.00, -0.00, 0.00},
          size = {5.00, 25.00, 5.00},
          origin = {0.00, -0.50, -0.50},
          shape = "cube.mesh",
          uv_map = {20.00, 0.00},
          color_primary = {233.00, 233.00, 233.00},
          nodes = {
            {
              name = "wand2",
              position = {0.00, 25.00, 0.00},
              rotation = {-90.00, -0.00, 0.00},
              size = {5.00, 25.00, 5.00},
              origin = {0.00, -0.50, 0.50},
              shape = "cube.mesh",
              uv_map = {20.00, 0.00},
              color_primary = {233.00, 233.00, 233.00},
              nodes = {
                {
                  name = "wand3",
                  position = {0.00, 17.50, -13.00},
                  rotation = {90.00, -0.00, 0.00},
                  size = {5.00, 15.00, 5.00},
                  origin = {0.00, -0.50, 0.00},
                  shape = "cube.mesh",
                  uv_map = {20.00, 0.00},
                  color_primary = {233.00, 233.00, 233.00},
                  nodes = {
                    {
                      name = "magic_ball",
                      animated = true,
                      position = {0.00, -9.00, 10.00},
                      rotation = {0.00, -0.00, -156.00},
                      size = {10.00, 10.00, 10.00},
                      shape = "cube.mesh",
                      uv_map = {20.00, 30.00},
                      color_primary = {0.00, 255.00, 180.00},
                      nodes = {
                        {
                          name = "light_5",
                          type = "light",
                          shape = "sphere.mesh",
                          shader = "point_light.shader.lua",
                          ambient = {0.00, 21.11, 26.90},
                          diffuse = {0.00, 255.00, 212.50},
                          specular = {0.00, 18.66, 33.35},
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
        },
      },
    },
  },
}
