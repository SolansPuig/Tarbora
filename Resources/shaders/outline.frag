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

in vec3 EdgeDistance;

void main()
{
  float nearDistance = min(min(EdgeDistance[0],EdgeDistance[1]),EdgeDistance[2]);
  float edgeIntensity = exp2(-0.1*nearDistance*nearDistance);

  FragColor = edgeIntensity * vec4(1, 1, 1, 1);
}
