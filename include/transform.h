#pragma once

#include <cmath>

inline float DegToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}
inline float RadToDeg(float radians) {
    return radians * 180.0f / M_PI;
}

struct Vec2
{
    float x;
    float y;

    Vec2(float _x, float _y) : x(_x), y(_y) {}

    // Operator overloads for vector arithmetic
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }
    Vec2 operator/(float scalar) const {
        return Vec2(x / scalar, y / scalar);
    }
    // Compound assignment operators
    Vec2 &operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vec2 &operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vec2 &operator*=(float scalar) {
        x *= scalar;
        y *= scalar;    
        return *this;
    }
    // Negation operator for reversing the direction of the vector
    Vec2 operator-() const {
        return Vec2(-x, -y);
    }
    // Length and normalization
    float Length() const {
        return std::sqrt(x * x + y * y);
    }
    Vec2 Normalized() const {
        float len = Length();
        if (len == 0) return Vec2(0, 0);
        return Vec2(x / len, y / len);
    }
};

struct Transform {
    Vec2 position;
    //angle in radians
    float angle;

    Vec2 Forward() const {
        return Vec2(std::cos(angle), std::sin(angle));
    }

    Vec2 Right() const {
        return Vec2(-std::sin(angle), std::cos(angle));
    }
};