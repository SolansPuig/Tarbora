#pragma once

#include "../Framework/MessageManager/MessageManager.hpp"
#include "math.pb.h"

namespace Tarbora {
    namespace Message {
        inline MathMessage::Vector toMessage(const glm::vec3 &vector)
        {
            MathMessage::Vector vec;
            vec.set_x(vector.x);
            vec.set_y(vector.y);
            vec.set_z(vector.z);
            return vec;
        }

        inline glm::vec3 toMath(const MathMessage::Vector &vector)
        {
            glm::vec3 vec;
            vec.x = vector.x();
            vec.y = vector.y();
            vec.z = vector.z();
            return vec;
        }

        inline MathMessage::Quaternion toMessage(const glm::quat &quaternion)
        {
            MathMessage::Quaternion quat;
            quat.set_x(quaternion.x);
            quat.set_y(quaternion.y);
            quat.set_z(quaternion.z);
            quat.set_w(quaternion.w);
            return quat;
        }

        inline glm::quat toMath(const MathMessage::Quaternion &quaternion)
        {
            glm::quat quat;
            quat.x = quaternion.x();
            quat.y = quaternion.y();
            quat.z = quaternion.z();
            quat.w = quaternion.w();
            return quat;
        }

        inline MathMessage::UvMap toMessage(const glm::tvec2<unsigned short> &uv_map)
        {
            MathMessage::UvMap vec;
            vec.set_x(uv_map.x);
            vec.set_y(uv_map.y);
            return vec;
        }

        inline glm::u16vec2 toMath(const MathMessage::UvMap &uv_map)
        {
            glm::u16vec2 vec;
            vec.x = uv_map.x();
            vec.y = uv_map.y();
            return vec;
        }

        inline MathMessage::Color toMessage(const glm::tvec3<unsigned char> &color)
        {
            MathMessage::Color vec;
            vec.set_r(color.r);
            vec.set_g(color.g);
            vec.set_b(color.b);
            return vec;
        }

        inline glm::u8vec3 toMath(const MathMessage::Color &color)
        {
            glm::u8vec3 vec;
            vec.r = color.r();
            vec.g = color.g();
            vec.b = color.b();
            return vec;
        }
    }

}
