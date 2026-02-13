#pragma once

#include "transform.h"

constexpr float piIncrement = (2*3.14159f )/360;
struct InputState;

class Level;

class Player {
public:
    Player(Vec2 startPos, float startAngle);
private:
    Transform   transform;
    Vec2        velocity;

    Vec2        targetDir;
    float       targetSpeed;
    float       currentSpeed;
    float       turnDirection;

    float       defaultSpeed = 1.5f;
    float       turnSpeed = 2.5f;
    float       collisionRadius = 0.05f;
public:
    Transform getTransform() { return transform; }

    float GetAngle() const { return transform.angle; }
    float GetAngleDeg() const { return RadToDeg(transform.angle); }

    Vec2 GetForward() const { return transform.Forward(); }

    Vec2 GetVelocity() const { return velocity; };

    void Move(const Vec2& vel);
    void Rotate(const float angleDelta);
    void Tick(const float deltaTime, const InputState& input, const Level& level);
};
