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
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

out vec3 EdgeDistance;

// uniform vec2 screenSize;

void main()
{
  vec2 screenSize = vec2(1600, 900);
  vec2 p0 = screenSize * gl_in[0].gl_Position.xy/gl_in[0].gl_Position.w;
  vec2 p1 = screenSize * gl_in[1].gl_Position.xy/gl_in[1].gl_Position.w;
  vec2 p2 = screenSize * gl_in[2].gl_Position.xy/gl_in[2].gl_Position.w;
  vec2 v0 = p2-p1;
  vec2 v1 = p2-p0;
  vec2 v2 = p1-p0;
  float area = abs(v1.x*v2.y - v1.y * v2.x);

  gl_Position = gl_in[0].gl_Position;
  EdgeDistance = vec3(area/length(v0),0,0);
  EmitVertex();

  gl_Position = gl_in[1].gl_Position;
  EdgeDistance = vec3(0,area/length(v1),0);
  EmitVertex();

  gl_Position = gl_in[2].gl_Position;
  EdgeDistance = vec3(0,0,area/length(v2));
  EmitVertex();

  EndPrimitive();
}
