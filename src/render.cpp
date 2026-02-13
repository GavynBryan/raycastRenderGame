#include <SDL2/SDL.h>
#include <cmath>
#include "render.h"
#include "view.h"
#include "level.h"
#include "lighting.h"
#include <algorithm>

Renderer::Renderer(SDL_Window& window, int sWidth, int sHeight)
    : screenWidth(sWidth), screenHeight(sHeight)
{
    renderer = SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, screenWidth, screenHeight);
}

Renderer::~Renderer()
{
    SDL_DestroyRenderer(renderer);
}

void Renderer::Render(const Level& level, const View& view)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    RenderFrameData frame {
        .screenWidth = screenWidth,
        .screenHeight = screenHeight,
        .depthBuffer = std::vector<float>(screenWidth)
    };
    RenderWalls(frame, level, view);

    SDL_RenderPresent(renderer);
}

void Renderer::RenderWalls(RenderFrameData& frame, const Level& level, const View& view)
{
    const float viewX = view.transform.position.x;
    const float viewY = view.transform.position.y;
    const float viewAngle = view.transform.angle;
    const float fov = view.fov;

    const float screenW = float(frame.screenWidth);
    const float screenH = float(frame.screenHeight);

    for (size_t i = 0; i < frame.screenWidth; i++)
    {
        float rayAngle = viewAngle - fov / 2 + fov * i / screenW;
        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        int mapX = int(viewX);
        int mapY = int(viewY);

        float deltaDistX = (rayDirX == 0) ? 1e30f : std::abs(1.0f / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : std::abs(1.0f / rayDirY);
        float sideDistX = (rayDirX < 0) ? (viewX - mapX) * deltaDistX : (mapX + 1.0f - viewX) * deltaDistX;
        float sideDistY = (rayDirY < 0) ? (viewY - mapY) * deltaDistY : (mapY + 1.0f - viewY) * deltaDistY;

        int stepX = (rayDirX < 0) ? -1 : 1;
        int stepY = (rayDirY < 0) ? -1 : 1;

        bool hit = false;
        int block = 0;
        int side = 0;

        while (!hit)
        {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            block = level.GetTile(mapX, mapY);
            if (block != 0) hit = true;
        }

        float distance = (side == 0)
            ? (mapX - viewX + (1 - stepX) / 2) / rayDirX
            : (mapY - viewY + (1 - stepY) / 2) / rayDirY;

        float normalizedDistance = distance * cos(rayAngle - viewAngle);
        frame.depthBuffer[i] = normalizedDistance;

        // --- per-tile height scaling ---
        float baseHeight = screenH / normalizedDistance;
        float tileHeightFactor = 1.0f;   // default full-height
        if (block == 2) tileHeightFactor = 0.5f; // shorter wall

        int wallBottom = static_cast<int>(screenH / 2 + baseHeight / 2);
        int wallTop = wallBottom - static_cast<int>(baseHeight * tileHeightFactor);

        SDL_Rect rect {
            .x = static_cast<int>(i),
            .y = wallTop,
            .w = 1,
            .h = wallBottom - wallTop
        };

        // --- shading & color ---
        float brightness = std::clamp(1.0f / (1.0f + normalizedDistance * shadowDistanceFactor), 0.0f, 1.0f);

        Uint8 r = 0, g = 255, b = 255; // default cyan
        if (block == 2) r = 255, g = 0, b = 0; // red

        r = static_cast<Uint8>(r * brightness);
        g = static_cast<Uint8>(g * brightness);
        b = static_cast<Uint8>(b * brightness);

        SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
        SDL_RenderDrawRect(renderer, &rect);
    }
}