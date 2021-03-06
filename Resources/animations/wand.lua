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

query = {
  time = 0,
  speed = 0.125,
}

idle = {
  duration = function() return 1/query.speed end,
  loop = true,

  nodes = {
    magic_ball = {
      position = function()
        return {
          0,
          2 * math.cos(2 * math.pi * 2 * query.speed * query.time + 15),
          0
        }
      end,
      rotation = function()
        return {
          0,
          180 * math.sin(2 * math.pi * query.speed * query.time),
          0
        }
      end,
    }
  }
}
