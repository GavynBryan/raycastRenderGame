#include <SDL2/SDL.h>
#pragma once

enum InputAction {
    MoveForward = SDL_SCANCODE_W,
    MoveBackward = SDL_SCANCODE_S,
    MoveLeft = SDL_SCANCODE_A,
    MoveRight = SDL_SCANCODE_D
};

struct InputState {
    bool left;
    bool right;
    bool forward;
    bool backward;

    float turnScalar = 0.0f;

    void PollInput(const Uint8* keystate) {
        left = keystate[MoveLeft];
        right = keystate[MoveRight];
        forward = keystate[MoveForward];
        backward = keystate[MoveBackward];
    }

    void Clear(){
        left = right = forward = backward = false;
        turnScalar = 0.0f;
    }
};