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

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transform;

out vec3 Position;

void main()
{
  vec4 pos = view * transform * vec4(aPos, 1.0);
  Position = (view * transform * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
  pos.z = -1.0;
  gl_Position = projection * pos;
}
