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

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

const int kernelSize = 8;
uniform vec3 samples[kernelSize];

uniform vec2 screenSize;
uniform mat4 projection;
uniform mat4 view;

const float radius = 0.05;
const float bias = 0.015;
const float rayThreshold = 0.15;

void main()
{
  vec2 noiseScale = vec2(screenSize/4.0);

  // get input for SSAO algorithm
  vec3 fragPos = texture(gPosition, TexCoords).xyz;
  vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
  if (normal == vec3(0.0f)) discard;
  vec3 randomVec = normalize(texture(texNoise, TexCoords * noiseScale).xyz);

  // create TBN change-of-basis matrix: from tangent-space to view-space
  vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
  vec3 bitangent = cross(normal, tangent);
  mat3 TBN = mat3(tangent, bitangent, normal);
  // iterate over the sample kernel and calculate occlusion factor
  float occlusion = 0.0;
  for(int i = 0; i < kernelSize; ++i)
  {
    // get sample position
    vec3 ray = TBN * samples[i]; // from tangent to view-space
    ray = fragPos + ray * radius;
    // project sample position (to sample texture) (to get position on screen/texture)
    vec4 offset = vec4(ray, 1.0);
    offset = projection * offset; // from view to clip-space
    offset.xyz /= offset.w; // perspective divide
    offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

    // get sample depth
    float sampleDepth = texture(gPosition, offset.xy).z; // get depth value of kernel sample

    // range check & accumulate
    float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
    occlusion += (sampleDepth >= ray.z + bias ? 1.0 : 0.0) * rangeCheck;
  }
  occlusion = 1.0 - (occlusion / kernelSize);

  FragColor = occlusion;
}
