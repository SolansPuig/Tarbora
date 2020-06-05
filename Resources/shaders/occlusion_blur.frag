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
out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main()
{
  vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
  float result = 0.0;
  for (int x = -2; x < 2; ++x)
  {
    for (int y = -2; y < 2; ++y)
    {
      vec2 offset = vec2(float(x), float(y)) * texelSize;
      result += texture(ssaoInput, TexCoords + offset).r;
    }
  }

  //FragColor = texture(ssaoInput, TexCoords).r;
  FragColor = result / (4.0 * 4.0);
}
