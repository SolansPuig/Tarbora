#pragma once

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
