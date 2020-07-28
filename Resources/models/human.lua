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

local query = {
  defaults = {
    skin_color = {255, 255, 255},
    shirt_color = {255, 255, 255},
    shirt_detail_color = {255, 255, 255},
    pants_color = {255, 255, 255},
    shoes_color = {255, 255, 255},
  }
}
setmetatable(query, {__index = query.defaults })

actor = {
  scale = 1,
  nodes = {
    {
      name = "body",
      scale = 1.00,
      size = {48.00, 26.00, 28.00},
      shape = "cube.mesh",
      uv_map = {0, 110},
      color_primary = query.skin_color,
      color_secondary = query.shirt_color,
      color_detail1 = query.shirt_detail_color,
      animated = true,
      nodes = {
        {
          name = "arm_l",
          scale = 1.00,
          origin = {-0.50, -0.40, 0.00},
          position = {38.00, 15.00, 0.00},
          rotation = {0.00, 10.00, -85.00},
          size = {30.00, 12.00, 12.00},
          shape = "cube_mirror.mesh",
          uv_map = {152, 0},
          color_primary = query.skin_color,
          color_secondary = query.shirt_color,
          color_detail = query.shirt_detail_color,
          animated = true,
          nodes = {
            {
              name = "forearm_l",
              scale = 1.00,
              origin = {-0.50, 0.00, 0.00},
              position = {27.00, 0.00, 0.00},
              rotation = {10.00, -20.00, -5.00},
              size = {30.00, 12.00, 12.00},
              shape = "cube_mirror.mesh",
              uv_map = {152, 24},
              color_primary = query.skin_color,
              color_secondary = query.shirt_color,
              color_detail = query.shirt_detail_color,
              animated = true,
              nodes = {
                {
                  name = "hand_l",
                  scale = 1.00,
                  origin = {0.00, 0.00, -0.20},
                  position = {15.00, -1.00, 0.00},
                  rotation = {4.00, 0.00, -8.00},
                  size = {14.00, 16.00, 16.00},
                  shape = "cube_mirror.mesh",
                  uv_map = {152, 48},
                  color_primary = query.skin_color,
                  animated = true,
                },
              }
            },
          }
        },
        {
          name = "arm_r",
          scale = 1.00,
          origin = {0.50, -0.40, 0.00},
          position = {-38.00, 15.00, 0.00},
          rotation = {0.00, -10.00, 85.00},
          size = {30.00, 12.00, 12.00},
          shape = "cube.mesh",
          uv_map = {152, 0},
          color_primary = query.skin_color,
          color_secondary = query.shirt_color,
          color_detail = query.shirt_detail_color,
          animated = true,
          nodes = {
            {
              name = "forearm_r",
              scale = 1.00,
              origin = {0.50, 0.00, 0.00},
              position = {-27.00, 0.00, 0.00},
              rotation = {10.00, 20.00, 5.00},
              size = {30.00, 12.00, 12.00},
              shape = "cube.mesh",
              uv_map = {152, 24},
              color_primary = query.skin_color,
              color_secondary = query.shirt_color,
              color_detail = query.shirt_detail_color,
              animated = true,
              nodes = {
                {
                  name = "hand_r",
                  scale = 1.00,
                  origin = {0.00, 0.00, -0.20},
                  position = {-15.00, -1.00, 0.00},
                  rotation = {4.00, -0.00, 8.00},
                  size = {14.00, 16.00, 16.00},
                  shape = "cube.mesh",
                  uv_map = {152, 48},
                  animated = true,
                  color_primary = query.skin_color,
                },
              }
            },
          }
        },
        {
          name = "belly",
          scale = 1.00,
          position = {0.00, -26.00, 0.00},
          size = {44.00, 26.00, 26.00},
          shape = "cube.mesh",
          uv_map = {0, 164},
          color_primary = query.skin_color,
          color_secondary = query.shirt_color,
          color_detail = query.shirt_detail_color,
          animated = true,
          nodes = {
            {
              name = "hip",
              scale = 1.00,
              position = {0.00, -18.00, 0.00},
              size = {44.00, 10.00, 26.00},
              shape = "cube.mesh",
              uv_map = {0, 216},
              color_primary = query.skin_color,
              color_secondary = query.pants_color,
              animated = true,
              nodes = {
                {
                  name = "thig_l",
                  scale = 1.00,
                  origin = {0.00, 0.40, 0.00},
                  position = {11.00, -15.00, 0.00},
                  size = {20.00, 36.00, 20.00},
                  shape = "cube.mesh",
                  uv_map = {152, 80},
                  color_primary = query.skin_color,
                  color_secondary = query.pants_color,
                  animated = true,
                  nodes = {
                    {
                      name = "leg_l",
                      scale = 1.00,
                      origin = {0.00, 0.50, 0.50},
                      position = {0.00, -36.00, 0.00},
                      size = {18.00, 40.00, 18.00},
                      shape = "cube.mesh",
                      uv_map = {152, 136},
                      color_primary = query.skin_color,
                      color_secondary = query.pants_color,
                      animated = true,
                      nodes = {
                        {
                          name = "foot_l",
                          scale = 1.00,
                          origin = {0.00, 0.00, -0.30},
                          position = {0.00, -22.00, 4.00},
                          size = {22.00, 10.00, 32.00},
                          shape = "cube.mesh",
                          uv_map = {140, 194},
                          color_primary = query.skin_color,
                          color_secondary = query.shoes_color,
                          animated = true,
                        },
                      }
                    },
                  }
                },
                {
                  name = "thig_r",
                  scale = 1.00,
                  origin = {0.00, 0.40, 0.00},
                  position = {-11.00, -15.00, 0.00},
                  size = {20.00, 36.00, 20.00},
                  shape = "cube.mesh",
                  uv_map = {152, 80},
                  color_primary = query.skin_color,
                  color_secondary = query.pants_color,
                  animated = true,
                  nodes = {
                    {
                      name = "leg_r",
                      scale = 1.00,
                      origin = {0.00, 0.50, 0.50},
                      position = {0.00, -36.00, 0.00},
                      size = {18.00, 40.00, 18.00},
                      shape = "cube.mesh",
                      uv_map = {152, 136},
                      color_primary = query.skin_color,
                      color_secondary = query.pants_color,
                      animated = true,
                      nodes = {
                        {
                          name = "foot_r",
                          scale = 1.00,
                          origin = {0.00, 0.00, -0.30},
                          position = {0.00, -22.00, 4.00},
                          size = {22.00, 10.00, 32.00},
                          shape = "cube.mesh",
                          uv_map = {140, 194},
                          color_primary = query.skin_color,
                          color_secondary = query.shoes_color,
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
        {
          name = "neck",
          scale = 1.00,
          position = {0.00, 14.00, 0.00},
          size = {18.00, 16.00, 18.00},
          shape = "cube.mesh",
          uv_map = {0, 76},
          animated = true,
          nodes = {
            {
              name = "head",
              scale = 1.00,
              origin = {0.00, -0.50, 0.00},
              position = {0.00, 20.00, 0.00},
              size = {38.00, 38.00, 38.00},
              shape = "cube.mesh",
              uv_map = {0, 0},
              animated = true,
              nodes = {
                {
                  name = "1st_person",
                  scale = 1.00,
                  type = "camera"
                },
                {
                  name = "3rd_person",
                  scale = 1.00,
                  origin = {0.00, -120.00, 800.00},
                  position = {0.00, 120.00, -800.00},
                  type = "camera"
                },
              }
            },
          }
        },
      }
    },
  }
}
