#pragma once

#include "../../Global.hpp"
#include "../proto/math.pb.h"

using tbMath::Vec3Message;
using tbMath::Vec4Message;
using tbMath::Mat3Message;
using tbMath::Mat4Message;

namespace Tarbora {

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
