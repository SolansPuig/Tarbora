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

phrases = {
  "Tarbora Game Engine",
  "What a beautiful day to play Tarbora!",
  "Tarbora: The Game Engine for Makers",
  "Tarborize Your Life!",
  "Now featuring extra bugs",
  "...!",
  "Doot",
  "Stay home, play Tarbora"
}

window = {
  title = function() return phrases[math.random(#phrases)] end,
  size = { 1600, 900 },
  postprocessShader = "shaders/postprocess.shader.lua"
}
