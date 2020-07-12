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

#ifndef __MATH_H_
#define __MATH_H_

#include "../CommonHeaders.hpp"

#include "../External/glm/glm/glm.hpp"
#include "../External/glm/glm/gtx/string_cast.hpp"
#include "../External/glm/glm/gtx/std_based_type.hpp"
#include "../External/glm/glm/gtc/type_ptr.hpp"
#include "../External/glm/glm/gtx/quaternion.hpp"
#include "../External/glm/glm/gtx/euler_angles.hpp"
#include "../External/glm/glm/gtx/matrix_decompose.hpp"

#endif // __MATH_H_

// inline glm::u8vec3 operator*(const glm::u8vec3 &a, char b) { return glm::u8vec3(b * a.x, b * a.y, b * a.z); }
// inline glm::u8vec3 operator*(char a, const glm::u8vec3 &b) { return b * a; }
// inline glm::u8vec3 operator/(const glm::u8vec3 &a, char b) { return glm::u8vec3(a.x/b, a.y/b, a.z/b); }
// inline glm::u16vec2 operator*(const glm::u16vec2 &a, short b) { return glm::u16vec2(b * a.x, b * a.y); }
// inline glm::u16vec2 operator*(short a, const glm::u16vec2 &b) { return b * a; }
// inline glm::u16vec2 operator/(const glm::u16vec2 &a, short b) { return glm::u16vec2(a.x/b, a.y/b); }
//
// namespace Tarbora {
//     template<class T>
//     class Property
//     {
//     public:
//         Property() {}
//
//         Property(const T &value)
//         {
//             value_ = value;
//             target_time_ = 0.f;
//         }
//
//         virtual void update(float delta_time)
//         {
//             if (target_time_ > 0.f)
//             {
//                 elapsed_time_ += delta_time;
//                 interpolate(elapsed_time_/target_time_);
//             }
//         }
//
//         virtual void set(const T &value)
//         {
//             value_ = value;
//         }
//
//         void interpolateTo(const T &target_value, float target_time)
//         {
//             if (target_time > 0.f)
//             {
//                 old_value_ = value_;
//                 target_value_ = target_value;
//                 elapsed_time_ = 0.f;
//                 target_time_ = target_time;
//             }
//             else
//             {
//                 set(target_value);
//             }
//         }
//
//         const T& get() const { return value_; }
//
//         float length() { return glm::length(value_); }
//
//         Property<T> normalize() { return Property<T>(glm::normalize(value_)); }
//
//         operator std::string() { return glm::to_string(value_); }
//
//         operator T() { return value_; }
//
//     protected:
//         virtual void interpolate(float fraction)
//         {
//             if (fraction >= 1.f)
//             {
//                 set(target_value_);
//                 target_time_ = 0.f;
//             }
//             else
//             {
//                 set(old_value_ + fraction * (target_value_ - old_value_));
//             }
//         }
//
//         T value_;
//         T old_value_;
//         T target_value_;
//         float elapsed_time_;
//         float target_time_;
//     };
//
//     class Position : public Property<glm::vec3>
//     {
//     public:
//         Position(const glm::vec3 &value) : Property<glm::vec3>(value) {}
//         Position() { value_ = glm::vec3(0.f); }
//
//         void x(float x) { value_.x = x; }
//         float x() const { return value_.x; }
//         void y(float y) { value_.y = y; }
//         float y() const { return value_.y; }
//         void z(float z) { value_.z = z; }
//         float z() const { return value_.z; }
//
//         friend Position operator+(const Position &a, const Position &b) { return Position(a.value_ * b.value_); }
//         friend Position operator-(const Position &a, const Position &b) { return Position(a.value_ - b.value_); }
//         friend Position operator*(const Position &a, const Position &b) { return Position(a.value_ * b.value_); }
//         friend Position operator/(const Position &a, const Position &b) { return Position(a.value_ / b.value_); }
//         friend Position operator*(const Position &a, float b) { return Position(a.get() * b); }
//         friend Position operator*(float a, const Position &b) { return Position(b * a); }
//         friend Position operator/(const Position &a, float b) { return Position(a.get() / b); }
//     };
//
//     class Quaternion;
//
//     class Rotation : public Property<glm::vec3>
//     {
//     public:
//         Rotation(const glm::vec3 &value) : Property<glm::vec3>(value) {}
//         Rotation() { value_ = glm::vec3(0.f); }
//         Rotation(const Quaternion &quat);
//
//         void x(float x) { value_.x = x; }
//         float x() const { return value_.x; }
//         void y(float y) { value_.y = y; }
//         float y() const { return value_.y; }
//         void z(float z) { value_.z = z; }
//         float z() const { return value_.z; }
//
//         friend Rotation operator+(const Rotation &a, const Rotation &b);
//         friend Rotation operator*(const Rotation &a, float b) { return Rotation(a.get() * b); }
//         friend Rotation operator*(float a, const Rotation &b) { return Rotation(b * a); }
//         friend Rotation operator/(const Rotation &a, float b) { return Rotation(a.get() / b); }
//
//     protected:
//         virtual void interpolate(float fraction);
//     };
//
//     class Direction : public Property<glm::vec3>
//     {
//     public:
//         Direction(const glm::vec3 &value) : Property<glm::vec3>(value) {}
//         Direction() { value_ = glm::vec3(0.f); }
//         Direction(const Quaternion &quat);
//
//         void x(float x) { value_.x = x; }
//         float x() const { return value_.x; }
//         void y(float y) { value_.y = y; }
//         float y() const { return value_.y; }
//         void z(float z) { value_.z = z; }
//         float z() const { return value_.z; }
//
//         friend Direction operator+(const Direction &a, const Direction &b);
//         friend Direction operator*(const Direction &a, float b) { return Direction(a.get() * b); }
//         friend Direction operator*(float a, const Direction &b) { return Direction(b * a); }
//         friend Direction operator/(const Direction &a, float b) { return Direction(a.get() / b); }
//
//     protected:
//         virtual void interpolate(float fraction);
//     };
//
//     class Scale : public Property<glm::vec3>
//     {
//     public:
//         Scale(const glm::vec3 &value) : Property<glm::vec3>(value) {}
//         Scale() { value_ = glm::vec3(1.f); }
//
//         void x(float x) { value_.x = x; }
//         float x() const { return value_.x; }
//         void y(float y) { value_.y = y; }
//         float y() const { return value_.y; }
//         void z(float z) { value_.z = z; }
//         float z() const { return value_.z; }
//
//         friend Scale operator+(const Scale &a, const Scale &b) { return Scale(a.value_ * b.value_); }
//         friend Scale operator-(const Scale &a, const Scale &b) { return Scale(a.value_ - b.value_); }
//         friend Scale operator*(const Scale &a, const Scale &b) { return Scale(a.value_ * b.value_); }
//         friend Scale operator/(const Scale &a, const Scale &b) { return Scale(a.value_ / b.value_); }
//         friend Scale operator*(const Scale &a, float b) { return Scale(a.get() * b); }
//         friend Scale operator*(float a, const Scale &b) { return Scale(b * a); }
//         friend Scale operator/(const Scale &a, float b) { return Scale(a.get() / b); }
//     };
//
//     class UvMap : public Property<glm::u16vec2>
//     {
//     public:
//         UvMap(const glm::u16vec2 &value) : Property<glm::u16vec2>(value) {}
//         UvMap() { value_ = glm::u16vec2(0); }
//
//         void x(short x) { value_.x = x; }
//         short x() const { return value_.x; }
//         void y(short y) { value_.y = y; }
//         short y() const { return value_.y; }
//
//         friend UvMap operator+(const UvMap &a, const UvMap &b) { return UvMap(a.value_ * b.value_); }
//         friend UvMap operator-(const UvMap &a, const UvMap &b) { return UvMap(a.value_ - b.value_); }
//         friend UvMap operator*(const UvMap &a, const UvMap &b) { return UvMap(a.value_ * b.value_); }
//         friend UvMap operator/(const UvMap &a, const UvMap &b) { return UvMap(a.value_ / b.value_); }
//         friend UvMap operator*(const UvMap &a, short b) { return UvMap(a.get() * b); }
//         friend UvMap operator*(short a, const UvMap &b) { return UvMap(b * a); }
//         friend UvMap operator/(const UvMap &a, short b) { return UvMap(a.get() / b); }
//     };
//
//     class Color : public Property<glm::u8vec3>
//     {
//     public:
//         Color(const glm::u8vec3 &value) : Property<glm::u8vec3>(value) {}
//         Color() { value_ = glm::u8vec3(255); }
//
//         void r(char r) { value_.x = r; }
//         char r() const { return value_.x; }
//         void g(char g) { value_.y = g; }
//         char g() const { return value_.y; }
//         void b(char b) { value_.z = b; }
//         char b() const { return value_.z; }
//
//         friend Color operator+(const Color &a, const Color &b) { return Color(a.value_ * b.value_); }
//         friend Color operator-(const Color &a, const Color &b) { return Color(a.value_ - b.value_); }
//         friend Color operator*(const Color &a, const Color &b) { return Color(a.value_ * b.value_); }
//         friend Color operator/(const Color &a, const Color &b) { return Color(a.value_ / b.value_); }
//         friend Color operator*(const Color &a, char b) { return Color(a.get() * b); }
//         friend Color operator*(char a, const Color &b) { return Color(b * a); }
//         friend Color operator/(const Color &a, char b) { return Color(a.get() / b); }
//     };
//
//     class Transform : public Property<glm::mat4>
//     {
//     public:
//         Transform(const glm::mat4 &value) : Property<glm::mat4>(value) {}
//         Transform(const Position &position, const Rotation &rotation, const Scale &scale, const Position &origin=Position());
//         Transform() { value_ = glm::mat4(1.f); }
//
//         const glm::mat4& get() const { return value_; }
//
//         friend Transform operator*(const Transform &a, const Transform &b) { return Transform(a.value_ * b.value_); }
//         friend Position operator*(const Position &a, const Transform &b) { return Position(glm::vec4(a.get(), 0.f) * b.value_); }
//         friend Rotation operator*(const Rotation &a, const Transform &b) { return Rotation(glm::vec4(a.get(), 0.f) * b.value_); }
//         friend Scale operator*(const Scale &a, const Transform &b) { return Scale(glm::vec4(a.get(), 0.f) * b.value_); }
//     };
//
//     Quaternion mix(const Quaternion &a, const Quaternion &b, float fraction);
//
//     class Quaternion : public Property<glm::quat>
//     {
//     public:
//         Quaternion(const glm::quat &quat) : Property<glm::quat>(quat) {}
//         Quaternion(const Rotation &rotation);
//         Quaternion(const Direction &direction);
//         Quaternion() { value_ = glm::quat(); }
//
//         void x(float x) { value_.x = x; }
//         float x() const { return value_.x; }
//         void y(float y) { value_.y = y; }
//         float y() const { return value_.y; }
//         void z(float z) { value_.z = z; }
//         float z() const { return value_.z; }
//         void w(float w) { value_.w = w; }
//         float w() const { return value_.w; }
//
//         friend Quaternion operator*(const Quaternion &a, const Quaternion &b) { return Quaternion(a.value_ * b.value_); }
//
//     protected:
//         virtual void interpolate(float fraction);
//     };
// }
