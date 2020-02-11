#pragma once

#include "Logger.hpp"
#include "External/glm/glm/glm.hpp"
#include "External/glm/glm/gtx/string_cast.hpp"
#include "External/glm/glm/gtx/transform2.hpp"
#include "External/glm/glm/gtx/euler_angles.hpp"
#include "External/glm/glm/gtc/type_ptr.hpp"
#include "External/glm/glm/gtc/type_ptr.hpp"
#include "External/tracy/Tracy.hpp"
#include "External/NamedType/named_type.hpp"

#define INVALID_ID 0
#define SKY_ID 10
#define MAIN_CAMERA_ID 15
typedef std::string ComponentId;
typedef std::string ActorId;
typedef unsigned int ClientId;
typedef std::string MessageSubject;

using Position = fluent::NamedType<glm::vec3, struct PositionT>;
using Rotation = fluent::NamedType<glm::vec3, struct RotationT>;
using Direction = fluent::NamedType<glm::vec3, struct DirectionT>;
using RotationMat = fluent::NamedType<glm::mat3, struct RotationMatT>;
