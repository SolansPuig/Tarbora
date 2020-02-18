// #pragma once
//
// #include "../CommonHeaders.hpp"
// #include "../Global.hpp"
//
// #include "glm/glm.hpp"
// #include "glm/gtx/string_cast.hpp"
// #include "glm/gtx/std_based_type.hpp"
//
// inline glm::u8vec3 operator*(const glm::u8vec3 &a, char b) { return glm::u8vec3(b * a.x, b * a.y, b * a.z); }
// inline glm::u8vec3 operator*(char a, const glm::u8vec3 &b) { return b * a; }
// inline glm::u8vec3 operator/(const glm::u8vec3 &a, char b) { return glm::u8vec3(a.x/b, a.y/b, a.z/b); }
// inline glm::u16vec2 operator*(const glm::u16vec2 &a, short b) { return glm::u16vec2(b * a.x, b * a.y); }
// inline glm::u16vec2 operator*(short a, const glm::u16vec2 &b) { return b * a; }
// inline glm::u16vec2 operator/(const glm::u16vec2 &a, short b) { return glm::u16vec2(a.x/b, a.y/b); }
//
// namespace Tarbora {
//     class Position {
//     public:
//         Position(float p=0.f) { value_ = glm::vec3(p, p, p); }
//         Position(float x, float y, float z) { value_ = glm::vec3(x, y, z); }
//         Position(const glm::vec3 vec);
//
//         const glm::vec3& get() { return value_; }
//
//         Position operator+(const Position &other) { return Position(value_ + other.value_); }
//         Position operator-(const Position &other) { return Position(value_ - other.value_); }
//
//         friend Position operator*(const Position &p, const Scale &s) { return Position(p.value_ * s.get()); }
//         friend Position operator*(const Scale &s, const Position &p) { return Position(s.get() * p.value_); }
//         friend Position operator/(const Position &p, const Scale &s) { return Position(p.value_ / s.get()); }
//         friend Position operator/(const Scale &s, const Position &p) { return Position(s.get() / p.value_); }
//
//         friend Position operator+(const Position &p, float f) { return Position(p.value_ + f); }
//         friend Position operator+(float f, const Position &p) { return Position(f + p.value_); }
//         friend Position operator-(const Position &p, float f) { return Position(p.value_ - f); }
//         friend Position operator-(float f, const Position &p) { return Position(f - p.value_); }
//         friend Position operator*(const Position &p, float f) { return Position(p.value_ * f); }
//         friend Position operator*(float f, const Position &p) { return Position(f * p.value_); }
//         friend Position operator/(const Position &p, float f) { return Position(p.value_ / f); }
//         friend Position operator/(float f, const Position &p) { return Position(f / p.value_); }
//
//         friend Position operator*(const Position &p, const Scale &s) { return Position(p.value_ * s.get()); }
//         friend Position operator*(const Scale &s, const Position &p) { return Position(s.get() * p.value_); }
//         friend Position operator/(const Position &p, const Scale &s) { return Position(p.value_ / s.get()); }
//         friend Position operator/(const Scale &s, const Position &p) { return Position(s.get() / p.value_); }
//
//     private:
//         glm::vec3 value_;
//     };
