#include <exception>
#include <cmath>
#include <string>
#include <stack>
#include <iostream>
#include <SDL.h>

//#include "include/player.h"

const int screenW = 800,
    screenH = 600;
const int mapW = 30,
    mapH = 30;
const float fov = 3.14159f/3;
const float piIncrement = (2*3.14159f )/360;
const float rotateSpeed = 135.0f;
const float moveSpeed = 2.0f;

const int worldMap[mapW][mapH] ={
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
{1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1},
{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1},
{1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
{1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
{1,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1,1},
{1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,0,1,1,1,1,1,1,1,0,0,0,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


SDL_Window &CreateWindow()
{
    SDL_Init(SDL_INIT_VIDEO);
    return *SDL_CreateWindow("One button game jam", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_FULLSCREEN);
}

float playerAngle = 45.0f * M_PI / 180.0f;
float playerX = 10;
float playerY = 4;



auto key = SDLK_UP;

void PerformRaycast(SDL_Renderer *renderer)
{
    for(size_t i = 0; i < screenW; i++){
        float angle = playerAngle - fov/2 + fov*i/float(screenW);
        float increment = .0001;
        for(float c = 0; c<30; c += increment){
            float x = playerX + c * cos(angle);
            float y = playerY + c * sin(angle);

            auto normalizedAngle = c*cos(angle - playerAngle);

            increment += 0.00025f;

            int block = worldMap[(int)x][(int)y];
            if(block == 1){
                size_t columnHeight = screenH/normalizedAngle;
                SDL_Rect rect {i, screenH/2 - columnHeight/2, 1, columnHeight};
                if(normalizedAngle <= 4) SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
                else SDL_SetRenderDrawColor(renderer, 0x00, 223, 223, 0xFF);
                SDL_RenderDrawRect(renderer, &rect);
                break;
            }
            if(block == 2){
                size_t columnHeight = screenH/normalizedAngle;
                SDL_Rect rect {i, screenH/2 - columnHeight/2, 1, columnHeight};
                SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderDrawRect(renderer, &rect);
                break;
            }
        }
    }
}

int main()
{
    SDL_Window& mWindow = CreateWindow();
    SDL_Renderer* mRenderer = SDL_CreateRenderer(&mWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(mRenderer, screenW, screenH);

    SDL_Event e; bool quit = false;

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    double deltatime = 0;
    while(!quit){
        last = now;
        now = SDL_GetTicks();
        deltatime = (double)(now - last)/1000;

        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE) quit = true;
                    break;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if(keystate[SDL_SCANCODE_LEFT])
            playerAngle -= piIncrement * rotateSpeed * deltatime;

        if(keystate[SDL_SCANCODE_RIGHT])
            playerAngle += piIncrement * rotateSpeed * deltatime;

        if(keystate[SDL_SCANCODE_UP]){
            playerX += (moveSpeed * cos(playerAngle)) * deltatime;
            playerY += (moveSpeed * sin(playerAngle)) * deltatime;
        }

        if(keystate[SDL_SCANCODE_DOWN]){
            playerX -= (moveSpeed * cos(playerAngle)) * deltatime;
            playerY -= (moveSpeed * sin(playerAngle)) * deltatime;
        }

        if(playerAngle > 2 * M_PI) playerAngle -= 2 * M_PI;
        if(playerAngle < 0) playerAngle += 2 * M_PI;
        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        PerformRaycast(mRenderer);
        SDL_RenderPresent(mRenderer);
    }
    SDL_DestroyWindow(&mWindow);
    SDL_Quit();
}
