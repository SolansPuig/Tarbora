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

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D light;

void main()
{
  float gamma = 2.2;

  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;
  vec3 Albedo = pow(texture(gColorSpec, TexCoords).rgb, gamma);
  vec3 Light = pow(texture(light, TexCoords).rgb, gamma);

  FragColor = vec4(
    pow(vec3(Albedo * Light), vec3(1.0/gamma), 1.0));
}
