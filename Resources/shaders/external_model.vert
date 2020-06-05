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
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aVertex;
layout (location = 3) in mat4 aTransform;
layout (location = 7) in vec2 aUV;
layout (location = 8) in vec3 aMeshSize;
layout (location = 9) in vec3 aTexSize;
layout (location = 10) in vec3 aPrimary;
layout (location = 11) in vec3 aSecondary;
layout (location = 12) in vec3 aDetail;
layout (location = 13) in vec3 aDetail2;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;
uniform int pixelDensity;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec3 colorPrimary;
out vec3 colorSecondary;
out vec3 colorDetail;
out vec3 colorDetail2;

void main()
{
  colorPrimary = aPrimary;
  colorSecondary = aSecondary;
  colorDetail = aDetail;
  colorDetail2 = aDetail2;

  vec4 pos = view * aTransform * vec4(aPos, 1.0f);
  FragPos = pos.xyz;
  Normal = vec3(transpose(inverse(view * aTransform)) * vec4(aNormal, 0.0));
  gl_Position = projection * pos;
  TexCoords = aVertex;
}
