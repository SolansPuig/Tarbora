#pragma once

#include "../Framework/MessageManager.hpp"
#include "proto/math.pb.h"

using namespace tbMath;

namespace Tarbora {
    Vec2Message Vec2(glm::vec2 data);
    Vec2Message Vec2(std::string data);

    glm::vec2 Vec2toGLM(Vec2Message data);

    Vec3Message Vec3(glm::vec3 data);
    Vec3Message Vec3(std::string data);

    glm::vec3 Vec3toGLM(Vec3Message data);

    Vec4Message Vec4(glm::vec4 data);
    Vec4Message Vec4(std::string data);

    glm::vec4 Vec4toGLM(Vec4Message data);

    Mat3Message Mat3(glm::mat3 data);
    Mat3Message Mat3(std::string data);

    glm::mat3 Mat3toGLM(Mat3Message data);

    Mat4Message Mat4(glm::mat4 data);
    Mat4Message Mat4(std::string data);

    glm::mat4 Mat4toGLM(Mat4Message data);
}
