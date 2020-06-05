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
uniform sampler2D colorTint;

in vec3 colorPrimary;
in vec3 colorSecondary;
in vec3 colorDetail;
in vec3 colorDetail2;

in vec2 TexCoord;

void main()
{
  vec4 fragTexture = texture(albedo, TexCoord);
  vec4 colorTintTexture = texture(colorTint, TexCoord);
  if (fragTexture.a == 0.0){
    discard;
    return;
  }

  vec3 primary = colorPrimary * colorTintTexture.r + (1. - colorTintTexture.r);
  vec3 secondary = colorSecondary * colorTintTexture.g + (1. - colorTintTexture.g);
  vec3 detail = colorDetail * colorTintTexture.b + (1. - colorTintTexture.b);
  vec3 detail2 = colorDetail2 * colorTintTexture.a + (1. - colorTintTexture.a);

  FragColor = vec4(
    clamp(fragTexture.rgb * primary * secondary * detail * detail2, 0.0, 1.0),
    1.0
  );
}
