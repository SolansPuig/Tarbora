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
out vec3 FragColor;

uniform sampler2D bloom;

in vec2 TexCoords;

uniform bool horizontal;
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
  vec2 tex_offset = 1.0 / textureSize(bloom, 0);
  vec3 result = texture(bloom, TexCoords).rgb * weight[0];
  if (horizontal)
  {
    for (int i = 1; i < 3; ++i)
    {
      result += texture(bloom, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
      result += texture(bloom, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
  }
  else
  {
    for (int i = 1; i < 3; ++i)
    {
      result += texture(bloom, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
      result += texture(bloom, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
    }
  }

  FragColor = result;
}
