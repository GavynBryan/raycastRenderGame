#pragma once

#include "transform.h"

class Player {
private:
    Transform*  transform;
    Vec2        velocity;

    Vec2        targetDir;
    float       targetSpeed;

    float       defaultSpeed = 5.0f;
public:
    Transform* getTransform() { return transform; }
    Vec2 GetVelocity() const { return velocity; }

    void SetVelocity(const Vec2& vel);
    void Tick(float deltaTime);
};
