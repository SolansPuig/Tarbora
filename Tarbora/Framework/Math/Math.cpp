// #include "Math.hpp"
//
// namespace Tarbora {
//     Rotation::Rotation(const Quaternion &quat)
//     {
//         value_ = glm::degrees(glm::eulerAngles(quat.get()));
//     }
//
//     Rotation operator+(const Rotation &a, const Rotation &b)
//     {
//         return Rotation(Quaternion(a)*Quaternion(b));
//     }
//
//     void Rotation::interpolate(float fraction)
//     {
//         if (fraction >= 1.f)
//         {
//             set(target_value_);
//             target_time_ = 0.f;
//         }
//         else
//         {
//             glm::quat old(glm::radians(old_value_));
//             glm::quat target(glm::radians(target_value_));
//             set(glm::degrees(glm::eulerAngles(mix(old, target, fraction))));
//         }
//     }
//
//     Direction::Direction(const Quaternion &quat)
//     {
//         value_ = glm::degrees(glm::eulerAngles(quat.get()));
//     }
//
//     Direction operator+(const Direction &a, const Direction &b)
//     {
//         return Direction(Quaternion(a)*Quaternion(b));
//     }
//
//     void Direction::interpolate(float fraction)
//     {
//         if (fraction >= 1.f)
//         {
//             set(target_value_);
//             target_time_ = 0.f;
//         }
//         else
//         {
//             glm::quat old(glm::radians(old_value_));
//             glm::quat target(glm::radians(target_value_));
//             set(glm::degrees(glm::eulerAngles(mix(old, target, fraction))));
//         }
//     }
//
//     Transform::Transform(const Position &position, const Rotation &rotation, const Scale &scale, const Position &origin)
//     {
//         Position scaled_origin = Position(origin.get() * scale.get());
//         value_ = glm::translate(glm::mat4(1.f), position.get() + scaled_origin.get());
//         value_ = glm::rotate(value_, glm::radians(rotation.get().x), glm::vec3(1.f, 0.f, 0.f));
//         value_ = glm::rotate(value_, glm::radians(rotation.get().y), glm::vec3(0.f, 1.f, 0.f));
//         value_ = glm::rotate(value_, glm::radians(rotation.get().z), glm::vec3(0.f, 0.f, 1.f));
//         value_ = glm::scale(value_, scale.get());
//         value_ = glm::translate(value_, -scaled_origin.get());
//     }
//
//     Quaternion::Quaternion(const Rotation &rotation)
//     {
//         value_ = glm::quat(glm::radians(rotation.get()));
//     }
//
//     Quaternion::Quaternion(const Direction &Direction)
//     {
//         value_ = glm::quat(glm::radians(Direction.get()));
//     }
//
//     Quaternion mix(const Quaternion &a, const Quaternion &b, float fraction)
//     {
//         return Quaternion(glm::mix(a.get(), b.get(), fraction));
//     }
//
//     void Quaternion::interpolate(float fraction)
//     {
//         if (fraction >= 1.f)
//         {
//             set(target_value_);
//             target_time_ = 0.f;
//         }
//         else
//         {
//             set(mix(old_value_, target_value_, fraction));
//         }
//     }
// }
