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

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssao;

uniform mat4 view;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform vec3 direction;

void main()
{
  float gamma = 2.2;

  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb;
  float Specular = texture(gColorSpec, TexCoords).a;
  float AmbientOcclusion = texture(ssao, TexCoords).r;

  vec3 lightDir = (view * vec4(normalize(-direction), 0.0)).xyz;
  vec3 viewDir = normalize(-FragPos);
  vec3 halfwayDir = reflect(-lightDir, Normal);

  vec3 ambient = ambient * AmbientOcclusion;
  vec3 diffuse = max(dot(Normal, lightDir), 0.0) * diffuse;
  vec3 specular = pow(max(dot(viewDir, halfwayDir), 0.0), 16.0) *
    specular * Specular;

  FragColor = vec4(ambient + diffuse + specular, 1.0);
}
