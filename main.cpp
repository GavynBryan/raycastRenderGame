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
#include <render.h>
#include <view.h>

const int screenW = 1920,
    screenH = 1080;
const int mapW = 30,
    mapH = 30;
const float fov = 3.14159f/3;
const float rotateSpeed = 135.0f;
const float moveSpeed = 2.0f;


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
    return *SDL_CreateWindow("2.5D render test", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, screenW, screenH, SDL_WINDOW_FULLSCREEN);
}

Level level;

Player player({10.0f, 4.0f}, DegToRad(45));

InputState inputState;


int main()
{
    SDL_Window& mWindow = CreateWindow();

    Renderer renderer(mWindow, screenW, screenH);

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
                    if(e.key.keysym.sym == SDLK_EQUALS) renderer.SetShadowDistanceFactor(renderer.GetShadowDistanceFactor() + 0.05f);
                    if(e.key.keysym.sym == SDLK_MINUS) renderer.SetShadowDistanceFactor(renderer.GetShadowDistanceFactor() - 0.05f);
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

        renderer.Render(level, player.GetView(fov));
    }
    SDL_DestroyWindow(&mWindow);
    SDL_Quit();
}
