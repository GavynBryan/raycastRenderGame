#include "player.h"
#include "input.h"
#include "level.h"

Player::Player(Vec2 startPos, float startAngle)
    : transform{Transform{startPos, startAngle}},
      velocity{0.0f, 0.0f},
      targetDir{0.0f, 0.0f},
      targetSpeed{0.0f},
      currentSpeed{0.0f} {}

void Player::Move(const Vec2& vel) {
    targetDir = vel;
    targetSpeed = defaultSpeed;
}

void Player::Rotate(const float angleDelta) {
    turnDirection = angleDelta;
}

void Player::Tick(const float deltaTime, const InputState& input, const Level& level) {
    Vec2 moveDir{0.0f, 0.0f};
    
    if(input.forward) {
        moveDir += GetForward();
    }
    else if(input.backward) {
        moveDir -= GetForward();
    }
    if(input.left) {
        moveDir -= transform.Right();
    }
    else if(input.right) {
        moveDir += transform.Right();
    }

    moveDir = moveDir.Normalized();
    Move(moveDir);

    Rotate(input.turnScalar);

    // Simple acceleration towards target velocity
    if (currentSpeed < targetSpeed) {
        currentSpeed += 5.0f * deltaTime; // Acceleration rate
        if (currentSpeed > targetSpeed) currentSpeed = targetSpeed;
    } else if (currentSpeed > targetSpeed) {
        currentSpeed -= 5.0f * deltaTime; // Deceleration rate
        if (currentSpeed < targetSpeed) currentSpeed = targetSpeed;
    }

    velocity = moveDir * currentSpeed;


    float predictedX = transform.position.x + velocity.x * deltaTime;
    float predictedY = transform.position.y + velocity.y * deltaTime;

    float bufferX = velocity.x > 0 ? collisionRadius : -collisionRadius;
    float bufferY = velocity.y > 0 ? collisionRadius : -collisionRadius;
    if (level.IsWall(static_cast<int>(predictedX + bufferX), 
                    static_cast<int>(transform.position.y))) {
        velocity.x = 0;
    }

    if (level.IsWall(static_cast<int>(transform.position.x),
                    static_cast<int>(predictedY + bufferY))) {
        velocity.y = 0;
    }

    // Update position
    transform.position.x += velocity.x * deltaTime;
    transform.position.y += velocity.y * deltaTime;

    // Update angle based on turn direction
    transform.angle += turnDirection * turnSpeed * deltaTime;
    // Keep angle within [0, 2*PI]
    if (transform.angle < 0) transform.angle += 2 * M_PI;
    if (transform.angle >= 2 * M_PI) transform.angle -= 2 * M_PI;

    // Reset turn direction for next tick
    turnDirection = 0.0f;

    // Reset target speed for next tick
    targetSpeed = 0.0f;
}