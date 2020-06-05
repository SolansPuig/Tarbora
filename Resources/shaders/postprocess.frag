/*********************************************************************
 * Copyright (C) 2020 Roger Solans Puig
 * Email: roger@solanspuig.cat
 *
 * This file is part of Tarbora. You can obtain a copy at
 * https://github.com/SolansPuig/Tarbora
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *********************************************************************/

#version 330 core
out vec4 FragColor;

uniform sampler2D scene;

in vec2 TexCoords;

void main()
{
  vec4 color = texture(scene, TexCoords);
  // vec4 offset = texture(scene, TexCoords - 0.002);
  // if (length(color - offset) > 0.14)
  // {
    // color = vec4(vec3(0.0), 1.0);
  // }
//
  FragColor = color;
}
