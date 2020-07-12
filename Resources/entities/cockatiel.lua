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
  type = {"bird"},
  model = {
    render_pass = 1,
    model = "cockatiel.lua",
    material = "cockatiel.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {0.75, 2, 0.75},
    density = 0,
    friction = 1,
    restitution = 0.3
  },
  animation = {
    file = "entities/cockatiel.lua"
  },
}

animation_controller = {
  start = { play = {{ name = "idle" }}},
  idle = {
    file = "cockatiel.lua", loop = true, base = true,
    events = {
      grabbed = { stop = {}, play = {{ name = "scared", fade_in = 0.1 }} },
      scratch = { stop = {}, play = {{ name = "scratching", fade_in = 0.3 }} },
      raise_crest = { stop = { stop_mode = 2, fade_out = 0.5}, play = {{ name = "raise_crest", fade_in = 0.5 }} },
    }
  },
  raise_crest = {
    file = "cockatiel.lua", loop = true, base = true,
    events = {
      grabbed = { stop = {}, play = {{ name = "scared", fade_in = 0.1 }} },
      scratch = { stop = {}, play = {{ name = "scratching", fade_in = 0.3 }} },
      lower_crest = { stop = {}, play = {{ name = "idle", fade_in = 0.5 }} },
    }
  },
  scared = {
    file = "cockatiel.lua", loop = true, base = true,
    events = {
      ungrabbed = { stop = { stop_mode = 2, fade_out = 0.3 }, play = {{ name = "idle" }} }
    }
  },
  scratching = {
    file = "cockatiel.lua", loop = true, base = true,
    events = {
      stop_scratching = { stop = { stop_mode = 2, fade_out = 0.5 }, play = {{ name = "idle" }} },
      grabbed = { stop = {}, play = {{ name = "scared", fade_in = 0.1 }} }
    }
  }
}
