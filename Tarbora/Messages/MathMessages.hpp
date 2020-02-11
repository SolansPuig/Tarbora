#pragma once

#include "../Framework/MessageManager.hpp"
#include "proto/math.pb.h"

namespace Tarbora {
    MathMessage::Vec2 Vec2(glm::vec2 data);
    MathMessage::Vec2 Vec2(std::string data);

    glm::vec2 Vec2toGLM(MathMessage::Vec2 data);

    MathMessage::Vec3 Vec3(glm::vec3 data);
    MathMessage::Vec3 Vec3(std::string data);

    glm::vec3 Vec3toGLM(MathMessage::Vec3 data);

    MathMessage::Vec4 Vec4(glm::vec4 data);
    MathMessage::Vec4 Vec4(std::string data);

    glm::vec4 Vec4toGLM(MathMessage::Vec4 data);

    MathMessage::Mat3 Mat3(glm::mat3 data);
    MathMessage::Mat3 Mat3(std::string data);

    glm::mat3 Mat3toGLM(MathMessage::Mat3 data);

    MathMessage::Mat4 Mat4(glm::mat4 data);
    MathMessage::Mat4 Mat4(std::string data);

    glm::mat4 Mat4toGLM(MathMessage::Mat4 data);
}
