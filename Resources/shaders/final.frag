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
uniform sampler2D bloom;

uniform float exposure;

in vec2 TexCoords;

void main()
{
  const float gamma = 2.2;

  vec3 Scene = texture(scene, TexCoords).rgb;
  vec3 Bloom = texture(bloom, TexCoords).rgb;

  Scene += Bloom;

  //vec3 mapped = Scene / (Scene + vec3(1.0));
  vec3 mapped = vec3(1.0) - exp(-Scene * exposure);
  mapped = pow(mapped, vec3(1.0 / gamma));

  FragColor = vec4(mapped, 1.0);
}
