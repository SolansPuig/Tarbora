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

in vec2 TexPos;
in vec2 TexSize;
in vec2 TexCoordX;
in vec2 TexCoordY;
in vec2 TexCoordZ;
in vec3 TexNormal;
in vec3 FragPos;
in vec3 Normal;
in vec3 colorPrimary;
in vec3 colorSecondary;
in vec3 colorDetail;
in vec3 colorDetail2;

uniform sampler2D albedo;
uniform sampler2D specular;
uniform sampler2D colorTint;

void main()
{
  vec2 texCoordZ = TexPos + mod(TexCoordZ, TexSize);
  vec2 texCoordY = TexPos + mod(TexCoordY, TexSize);
  vec2 texCoordX = TexPos + mod(TexCoordX, TexSize);
  vec3 blend = vec3(0);

  // Asymetric triplanar blend
  // vec2 blendXZ = abs(normalize(TexNormal.xz));
  // blend.x = max(0.0, blendXZ.x - 0.67);
  // blend.z = max(0.0, blendXZ.y - 0.67);
  // blend.xz /= max(0.00001, dot(blend.xz, vec2(1, 1)));
  // blend.y = clamp((abs(normalize(TexNormal.y)) - 0.675) * 80.0, 0.0, 1.0);
  // blend.xz *= (1.0 - blend.y);

  // Pow triplanar blend
  blend.x = pow(abs(TexNormal.x), 16);
  blend.y = pow(abs(TexNormal.y), 16);
  blend.z = pow(abs(TexNormal.z), 16);
  blend /= blend.x + blend.y + blend.z;

  gPosition = FragPos;
  gNormal = normalize(Normal);
  vec4 fragTexture =
    texture(albedo, texCoordZ) * blend.z +
    texture(albedo, texCoordY) * blend.y +
    texture(albedo, texCoordX) * blend.x;
  vec4 specularTexture =
    texture(specular, texCoordZ) * blend.z +
    texture(specular, texCoordY) * blend.y +
    texture(specular, texCoordX) * blend.x;
  vec4 colorTintTexture =
    texture(colorTint, texCoordZ) * blend.z +
    texture(colorTint, texCoordY) * blend.y +
    texture(colorTint, texCoordX) * blend.x;
  if (fragTexture.a == 0.0){
    discard;
  }

  vec3 primary = colorPrimary * colorTintTexture.r + (1. - colorTintTexture.r);
  vec3 secondary = colorSecondary * colorTintTexture.g + (1. - colorTintTexture.g);
  vec3 detail = colorDetail * colorTintTexture.b + (1. - colorTintTexture.b);
  vec3 detail2 = colorDetail2 * colorTintTexture.a + (1. - colorTintTexture.a);

  gColorSpec.rgb = fragTexture.rgb * primary * secondary * detail * detail2;
  gColorSpec.a = specularTexture.r;
}
