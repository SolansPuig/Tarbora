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
      shape = "cube.mesh",
      origin = {0, 0, 0},
      position = {0, 0, 0},
      rotation = {0, 0, 0},
      size = {100, 100, 100},
      uv_map = {0, 0},
      nodes = {
        {
          name = "light",
          type = "light",
          shape = "sphere.mesh",
          shader = "point_light.shader.lua",
          position = {0, 0, 154},
          attenuation = {1.43, 0.246, 0},
          ambient = {14, 0, 0},
          diffuse = {137, 0, 0},
          specular = {34, 33, 0}
        }
      }
    }
  }
}
