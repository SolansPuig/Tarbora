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

components = {
  type = {"human"},
  model = {
    render_pass = 1,
    model = "human.lua",
    material = "human.mat.lua"
  },
  animation = {
    file = "entities/human.lua"
  },
  rigidbody = {
    shape = "capsule",
    height = 1.4,
    radius = 0.5,
    density = 2
  },
  controller = {
    speed = 10.0,
    run_speed = 9.0,
  },
  sight = {
    eye_position = {0.0, 0.5, 0.0}
  },
  grab = {},
  inventory = {
    { name = "INVENTORY", slots = 10 },
    {
      name = "RIGHT HAND", node = "hand_r",
      offsets = {{-0.0, 0.0, 0.0}},
      orientations = {{90.0, 0.0, 180.0}}
    }
  }
}

animation_controller = {
  start = { play = {{ name = "idle" }} },
  idle = {
    file = "human2.lua", loop = true, base = true,
    events = {
      walk = { stop = {}, play = {{ name = "walk" }} },
      cast = { play = {{ name = "cast" }} }
    }
  },
  walk = {
    file = "human2.lua", loop = true, speed = 4, base = true,
    events = {
      stop = { stop = {stop_mode = 2, fade_out=0.5}, play = {{ name = "idle" }} },
      cast = { play = {{ name = "cast" }}  }
    }
  },
  cast = {
    file = "human2.lua", loop = false, speed = 1, base = false
  }
}
