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
layout (location = 13) in vec3 aEmissive;

uniform mat4 projection;
uniform mat4 view;
uniform sampler2D myTexture;
uniform int pixelDensity;

out vec2 TexPos;
out vec2 TexSize;
out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec3 colorPrimary;
out vec3 colorSecondary;
out vec3 colorDetail;
out vec3 colorEmissive;

void main()
{
  colorPrimary = aPrimary;
  colorSecondary = aSecondary;
  colorDetail = aDetail;
  colorEmissive = aEmissive;

  vec4 pos = view * aTransform * vec4(aPos, 1.0f);
  FragPos = pos.xyz;
  Normal = vec3(transpose(inverse(view * aTransform)) * vec4(aNormal, 0.0));
  gl_Position = projection * pos;

  vec3 meshSize = aMeshSize * pixelDensity;
  vec3 texSize = aTexSize * pixelDensity;
  vec2 uv = aUV * pixelDensity / 100.0;
  float face = aVertex.x;
  float vertex = aVertex.y;
  bool vertexX = (vertex == 0 || vertex == 3) ? true : false; // The vertex is on the left side?
  bool vertexY = (vertex == 0 || vertex == 1) ? true : false; // The vertex is on the up side?

  switch (int(face)) {
    case 0:
      TexPos.xy = uv.xy + texSize.z;
      TexSize.xy = texSize.xy;
      TexCoords.x = vertexX ? 2.0  : meshSize.x - 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.y - 2.0;
      break;
    case 1:
      TexPos.x = uv.x + texSize.z + 2*texSize.x;
      TexPos.y = uv.y + texSize.z;
      TexSize.xy = texSize.zy;
      TexCoords.x = vertexX ? 2.0 : meshSize.z - 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.y - 2.0;
      break;
    case 2:
      TexPos.x = uv.x + texSize.z + texSize.x;
      TexPos.y = uv.y + texSize.z;
      TexSize.xy = texSize.xy;
      TexCoords.x = vertexX ? meshSize.x - 2.0: 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.y - 2.0;
      break;
    case 3:
      TexPos.x = uv.x;
      TexPos.y = uv.y + texSize.z;
      TexSize.xy = texSize.zy;
      TexCoords.x = vertexX ? 2.0 : meshSize.z - 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.y - 2.0;
      break;
    case 4:
      TexPos.x = uv.x + texSize.z;
      TexPos.y = uv.y;
      TexSize.xy = texSize.xz;
      TexCoords.x = vertexX ? 2.0 : meshSize.x - 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.z - 2.0;
      break;
    case 5:
      TexPos.x = uv.x + texSize.z + texSize.x;
      TexPos.y = uv.y;
      TexSize.xy = texSize.xz;
      TexCoords.x = vertexX ? 2.0 : meshSize.x - 2.0;
      TexCoords.y = vertexY ? 2.0 : meshSize.z - 2.0;
      break;
  }

  TexPos /= textureSize(myTexture, 0);
  TexSize /= textureSize(myTexture, 0);
  TexCoords /= textureSize(myTexture, 0);
}
