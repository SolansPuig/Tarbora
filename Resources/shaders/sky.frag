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

in vec3 pos;
in vec2 TexCoord;

uniform vec3 horizonColor;
uniform vec3 skyColor;
uniform float horizonHeight;
uniform float exponent;

struct Sun {
    vec3 color;
    vec3 pos;
    float size;
    float sharpness;
};

uniform Sun sun[5];

void main()
{
  vec3 p = normalize(pos);
  float height = pow(clamp(horizonHeight - p.y, 0.0, 1.0), exponent);
  vec3 color = mix(skyColor/255.0, horizonColor/255.0, height);

  for (int i = 0; i < 5; ++i)
  {
    float d = clamp(
      pow(abs(distance(p, normalize(sun[i].pos)))*128/sun[i].size, sun[i].sharpness),
      0.0, 1.0
    );
    color = mix(sun[i].color/255.0, color, d);
  }
  FragColor = vec4(color, 1.0);
}
