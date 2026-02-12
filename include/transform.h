#include <cmath>

struct Vec2
{
    float x;
    float y;

    Vec2(float _x, float _y) : x(_x), y(_y) {}
};

struct Transform {
    Vec2 position;
    float angle;

    Vec2 Forward() const {
        return Vec2(std::cos(angle), std::sin(angle));
    }
};

extern float DegToRad(float degrees) {
    return degrees * M_PI / 180.0f;
}