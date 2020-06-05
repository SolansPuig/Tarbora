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
layout (location = 2) in vec2 aVertex;

out vec2 TexCoords;

void main()
{
  gl_Position = vec4(aPos, 1.0);
  float vertex = aVertex.y;
  TexCoords.x = (vertex == 0 || vertex == 3) ? 0 : 1;
  TexCoords.y = (vertex == 0 || vertex == 1) ? 0 : 1;
}
