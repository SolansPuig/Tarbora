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

#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include "Logger.hpp"
#include "Math/Math.hpp"
#include "Tracy.hpp"

#define UNUSED(X) (void)(X);
#define INVALID_ID 0
#define SKY_ID 10
#define MAIN_CAMERA_ID 15
typedef std::string ActorId;
typedef std::string ComponentId;
typedef unsigned int ClientId;
typedef unsigned int SubscriptorId;
typedef std::string MessageSubject;

#endif // __GLOBAL_H_
