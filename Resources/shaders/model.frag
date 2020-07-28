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
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColorSpec;
layout (location = 3) out vec4 gEmissive;

in vec2 TexPos;
in vec2 TexSize;
in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec3 colorPrimary;
in vec3 colorSecondary;
in vec3 colorDetail;
in vec3 colorDetail2;

uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D colorTint;
uniform sampler2D emissive;


void main()
{
  vec2 texCoord = TexPos + mod(TexCoords, TexSize);
  gPosition = FragPos;
  gNormal = normalize(Normal);
  vec4 fragTexture = texture(albedo, texCoord);
  vec4 specularTexture = texture(specular, texCoord);
  vec4 mask = texture(colorTint, texCoord);
  vec4 Emissive = texture(emissive, texCoord);
  if (fragTexture.a == 0.0){
    discard;
  }

  vec3 primary = colorPrimary * mask.r + (1. - mask.r);
  vec3 secondary = colorSecondary * mask.g + (1. - mask.g);
  vec3 detail = colorDetail * mask.b + (1. - mask.b);
  vec3 detail2 = colorDetail2 * mask.a + (1. - mask.a);

  gColorSpec.rgb = fragTexture.rgb * primary * secondary * detail * detail2;
  gColorSpec.a = specularTexture.r;
  gEmissive = Emissive;
}
