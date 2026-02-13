#include <exception>
#include <cmath>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SDL.h>

#include <player.h>
#include <input.h>
#include <level.h>
#include <transform.h>

const int screenW = 1920,
    screenH = 1080;
const int mapW = 30,
    mapH = 30;
const float fov = 3.14159f/3;
const float rotateSpeed = 135.0f;
const float moveSpeed = 2.0f;

std::vector<SDL_Rect> WallCache(screenW);

const MapArray worldMap ={
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

Level level;

Player player({10.0f, 4.0f}, DegToRad(45));


auto key = SDLK_UP;

void PerformRaycast(SDL_Renderer* renderer)
{
    const auto& playerTransform = player.getTransform();
    const float playerX = playerTransform.position.x;
    const float playerY = playerTransform.position.y;
    const float playerAngle = player.GetAngle();

    for (size_t i = 0; i < screenW; i++)
    {
        // compute ray angle for this column
        float rayAngle = playerAngle - fov / 2 + fov * i / float(screenW);
        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        // map square we're currently in
        int mapX = static_cast<int>(playerX);
        int mapY = static_cast<int>(playerY);

        // length of ray from current position to next x or y-side
        float sideDistX;
        float sideDistY;

        // distance from one x or y-side to next x or y-side
        float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);

        int stepX = (rayDirX < 0) ? -1 : 1;
        int stepY = (rayDirY < 0) ? -1 : 1;

        // initial side distances
        sideDistX = (rayDirX < 0) ? (playerX - mapX) * deltaDistX
                                   : (mapX + 1.0f - playerX) * deltaDistX;
        sideDistY = (rayDirY < 0) ? (playerY - mapY) * deltaDistY
                                   : (mapY + 1.0f - playerY) * deltaDistY;

        bool hit = false;
        int block = 0;
        int side = 0; // 0 = x side, 1 = y side

        // DDA loop
        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            block = level.GetTile(mapX, mapY);
            if (block != 0) hit = true;
        }

        // calculate distance to wall (with fish-eye correction)
        float distance = (side == 0)
            ? (mapX - playerX + (1 - stepX) / 2) / rayDirX
            : (mapY - playerY + (1 - stepY) / 2) / rayDirY;

        float normalizedDistance = distance * cos(rayAngle - playerAngle);
        size_t columnHeight = static_cast<size_t>(screenH / normalizedDistance);

        // update cached rect
        SDL_Rect& rect = WallCache[i];
        rect.x = i;
        rect.y = screenH / 2 - columnHeight / 2;
        rect.w = 1;
        rect.h = columnHeight;

        // distance-based shading
        float brightness = 1.0f / (1.0f + normalizedDistance * 0.1f);
        brightness = std::clamp(brightness, 0.0f, 1.0f);

        // base color per block type
        Uint8 baseR = 0, baseG = 255, baseB = 255; // default cyan wall
        if (block == 2) { baseR = 255; baseG = 0; baseB = 0; }

        // darken based on distance
        Uint8 r = static_cast<Uint8>(baseR * brightness);
        Uint8 g = static_cast<Uint8>(baseG * brightness);
        Uint8 b = static_cast<Uint8>(baseB * brightness);

        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderDrawRect(renderer, &rect);
    }
}

InputState inputState;


int main()
{
    SDL_Window& mWindow = CreateWindow();
    SDL_Renderer* mRenderer = SDL_CreateRenderer(&mWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(mRenderer, screenW, screenH);

    SDL_Event e; bool quit = false;

    Uint64 now = SDL_GetTicks();
    Uint64 last = 0;
    double deltatime = 0;

    level.SetMap(worldMap);
    level.AddSolid(0);
    level.AddSolid(1);

    SDL_SetRelativeMouseMode(SDL_TRUE);

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
                case SDL_MOUSEMOTION:
                    inputState.turnScalar = e.motion.xrel > 0 ? 1.0f : e.motion.xrel < 0 ? -1.0f : 0.0f;
                    break;
            }
        }

        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        inputState.PollInput(keystate);

        player.Tick(deltatime, inputState, level);

        inputState.Clear();

        SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(mRenderer);

        PerformRaycast(mRenderer);
        SDL_RenderPresent(mRenderer);
    }
    SDL_DestroyWindow(&mWindow);
    SDL_Quit();
}
