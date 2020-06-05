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

in vec3 position;

uniform sampler2D gPosition;
uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 view;

const float foam = 0.5;
const vec4 water_color = vec4(0.0, 0.9, 1.0, 0.5);
const vec4 foam_color = vec4(1.00, 1.0, 1.0, 1.0);

void main()
{
  vec4 pos = texture(gPosition, gl_FragCoord.xy/screenSize);
  float dist = 1.0 - length(pos.xyz - position)/foam;
  FragColor = mix(water_color, foam_color, max(dist, 0.0));
}
