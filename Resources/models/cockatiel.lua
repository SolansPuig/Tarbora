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
      position = {0.00, 80.00, 0.00},
      size = {28.00, 2.00, 2.00},
      shape = "cube.mesh",
      uv_map = {28, 64},
      color_primary = {255, 254, 254},
      color_secondary = {254, 255, 255},
      texture_size = {14.00, 2.00, 2.00},
      nodes = {
        {
          name = "bird",
          scale = 1.00,
          position = {1.00, 13.00, 0.00},
          rotation = {20.00, -0.00, 0.00},
          size = {10.00, 24.00, 10.00},
          shape = "cube.mesh",
          uv_map = {0, 22},
          nodes = {
            {
              name = "head",
              scale = 1.00,
              position = {0.00, 17.00, 0.00},
              rotation = {-10.00, -0.00, 0.00},
              size = {9.00, 11.00, 11.00},
              shape = "cube.mesh",
              uv_map = {0, 0},
              animated = true,
              nodes = {
                {
                  name = "beack",
                  scale = 1.00,
                  position = {0.00, -1.00, 6.40},
                  rotation = {-5.00, -0.00, 0.00},
                  size = {3.00, 5.00, 3.00},
                  shape = "cube.mesh",
                  uv_map = {40, 0},
                },
                {
                  name = "crest_1",
                  scale = 1.00,
                  origin = {0.00, -0.50, 0.00},
                  position = {0.00, 11.50, 4.00},
                  rotation = {-19.00, -0.00, 0.00},
                  size = {3.00, 13.00, 3.50},
                  shape = "cube.mesh",
                  uv_map = {40, 8},
                  animated = true,
                },
                {
                  name = "crest_2",
                  scale = 1.00,
                  origin = {0.00, -0.50, 0.00},
                  position = {0.00, 9.50, 1.00},
                  rotation = {-30.00, -0.00, 0.00},
                  size = {2.00, 10.00, 3.00},
                  shape = "cube.mesh",
                  uv_map = {40, 8},
                  texture_size = {3.00, 13.00, 3.50},
                  mesh_size = {3.0, 13.0, 3.5},
                  animated = true,
                },
                {
                  name = "crest_3",
                  scale = 1.00,
                  origin = {0.00, -0.50, 0.00},
                  position = {0.00, 7.50, -1.00},
                  rotation = {-45.00, -0.00, 0.00},
                  size = {2.00, 8.00, 3.00},
                  shape = "cube.mesh",
                  uv_map = {40, 8},
                  texture_size = {3.00, 13.00, 3.50},
                  mesh_size = {3.0, 13.0, 3.5},
                  animated = true,
                },
                {
                  name = "eye_l",
                  scale = 1.00,
                  position = {4.80, 0.50, 3.00},
                  rotation = {-20.00, -0.00, 0.00},
                  size = {1.00, 3.00, 3.00},
                  shape = "cube.mesh",
                  uv_map = {52, 0},
                },
                {
                  name = "eye_r",
                  scale = 1.00,
                  position = {-4.80, 0.50, 3.00},
                  rotation = {-20.00, -0.00, 0.00},
                  size = {1.00, 3.00, 3.00},
                  shape = "cube.mesh",
                  uv_map = {52, 0},
                },
              }
            },
            {
              name = "leg_l",
              scale = 1.00,
              position = {2.50, -11.80, 5.00},
              rotation = {-13.00, -0.00, 0.00},
              size = {3.50, 4.00, 4.00},
              shape = "cube_mirror.mesh",
              uv_map = {28, 56},
            },
            {
              name = "leg_r",
              scale = 1.00,
              position = {-2.50, -11.80, 5.00},
              rotation = {-13.00, -0.00, 0.00},
              size = {3.50, 4.00, 4.00},
              shape = "cube.mesh",
              uv_map = {28, 56},
              animated = true,
            },
            {
              name = "tail",
              scale = 1.00,
              origin = {0.00, 0.50, 0.00},
              position = {0.00, -23.00, -5.00},
              rotation = {-8.00, -0.00, 0.00},
              size = {14.00, 22.00, 5.00},
              shape = "double_plane.mesh",
              uv_map = {0, 56},
              texture_size = {14.00, 22.00, 0.00},
              mesh_size = {14.00, 22.00, 0.00},
            },
            {
              name = "wing_l",
              scale = 1.00,
              origin = {0.00, 0.50, 0.00},
              position = {5.50, 1.00, 0.00},
              rotation = {5.00, 10.00, 4.00},
              size = {2.00, 20.00, 8.00},
              shape = "cube_mirror.mesh",
              uv_map = {40, 25},
              animated = true,
              nodes = {
                {
                  name = "feathers_l",
                  scale = 1.00,
                  origin = {0.00, 0.00, 0.00},
                  position = {0.00, 0.00, 0.00},
                  rotation = {0.00, 90.00, 0.00},
                  size = {8.00, 20.00, 100.00},
                  shape = "double_plane.mesh",
                  uv_map = {0, 78},
                  texture_size = {18.00, 36.00, 0.00},
                  mesh_size = {18.00, 36.00, 0.00},
                  animated = true
                },
              }
            },
            {
              name = "wing_r",
              scale = 1.00,
              origin = {0.00, 0.50, 0.00},
              position = {-5.50, 1.00, 0.00},
              rotation = {5.00, -10.00, -4.00},
              size = {2.00, 20.00, 8.00},
              shape = "cube.mesh",
              uv_map = {40, 25},
              animated = true,
              nodes = {
                {
                  name = "feathers_r",
                  scale = 1.00,
                  origin = {0.00, 0.00, 0.00},
                  position = {0.00, 0.00, 0.00},
                  rotation = {0.00, 90.00, 0.00},
                  size = {8.00, 20.00, 100.00},
                  shape = "double_plane.mesh",
                  uv_map = {0, 78},
                  texture_size = {18.00, 36.00, 0.00},
                  mesh_size = {18.00, 36.00, 0.00},
                  animated = true
                },
              }
            },
          }
        },
        {
          name = "stand",
          scale = 1.00,
          position = {-12.60, -65.00, 0.00},
          size = {3.00, 132.00, 3.00},
          shape = "cube.mesh",
          uv_map = {60, 0},
          texture_size = {3.00, 60.00, 3.00},
          color_primary = {255, 254, 254},
        },
      }
    },
  }
}
