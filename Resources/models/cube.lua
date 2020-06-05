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
    shape = "cube.mesh",
    origin = {0, 0, 0},
    position = {0, 0, 0},
    rotation = {0, 0, 0},
    size = {100, 100, 100},
    -- color_primary = {
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end
    -- },
    -- color_secondary = {
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end
    -- },
    -- color_detail1 = {
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end,
    --     function() return math.random(0, 255) end
    -- },
    color_primary = {255, 255, 255},
    color_secondary = {0, 255, 255},
    color_detail1 = {255, 0, 255},
    uv_map = {0, 0}
  }
}
