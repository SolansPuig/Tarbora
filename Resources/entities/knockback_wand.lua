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
  type = {"weapon", "knockback", "magic"},
  model = {
    render_pass = 1,
    model = "mage_wand2.lua",
    material = "mage_wand.mat.lua"
  },
  rigidbody = {
    shape = "box",
    size = {0.05, 1.8, 0.05},
    density = 20000,
    friction = 0.9,
    restitution = 0.1
  },
  item = {
    name = "Knockback Wand",
    valid_groups = {"RIGHT HAND", "INVENTORY"},
    max_quantity = 1,
    offset = {0.0, -0.5, 0.0}
  },
  knockback = {
    strength = 500.0,
    range = 50.0
  },
  animation = {
    file = "entities/gravity_wand.lua"
  }
}
