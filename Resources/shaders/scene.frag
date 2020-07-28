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
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec3 BloomColor;

uniform sampler2D albedo;
uniform sampler2D light;
uniform sampler2D emissive;

in vec2 TexCoords;

void main()
{
  const float gamma = 2.2;

  vec3 Albedo = pow(texture(albedo, TexCoords).rgb, vec3(gamma));
  vec3 Light = texture(light, TexCoords).rgb;
  vec3 Emissive = texture(emissive, TexCoords).rgb;

  vec3 hdrColor = Albedo * (Light + Emissive);

  FragColor = vec4(hdrColor, 1.0);

  float brightness = dot(hdrColor + 10.0 * Emissive, vec3(0.2126, 0.7152, 0.0722));
  if (brightness > 1.0)
    BloomColor = FragColor.rgb;
  else
    BloomColor = vec3(0.0, 0.0, 0.0);
}
