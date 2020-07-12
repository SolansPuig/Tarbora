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
  type = {"magic", "projectile", "arcane"},
  model = {
    render_pass = 2,
    model = "orb.lua",
    material = "mage_wand.mat.lua"
  },
  rigidbody= {
    shape = "sphere",
    radius = 0.3,
    density = 90,
    friction = 0.5,
    restitution = 3.0
  },
}
