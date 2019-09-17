#pragma once
#include "Logger.hpp"
#include "External/glm/glm/glm.hpp"
#include "External/glm/glm/gtx/string_cast.hpp"
#include "External/glm/glm/gtc/type_ptr.hpp"
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <csignal>
#include <optional>

#define INVALID_ID 0
#define SKY_ID 10
#define MAIN_CAMERA_ID 15
typedef unsigned int ComponentId;
typedef unsigned int ActorId;