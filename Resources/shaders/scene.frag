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

uniform sampler2D albedo;
uniform sampler2D light;

in vec2 TexCoords;

void main()
{
  float gamma = 2.2;

  vec3 Albedo = pow(texture(albedo, TexCoords).rgb, vec3(gamma));
  vec3 Light = texture(light, TexCoords).rgb;

  FragColor = vec4(pow(vec3(Albedo * Light), vec3(1.0/gamma)), 1.0);
  //FragColor = vec4(Light, 1.0);
}
