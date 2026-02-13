#pragma once
#include <vector>

class Level;
class Player;
struct SDL_Renderer;
struct SDL_Window;
struct View;

struct RenderFrameData {
    int screenWidth;
    int screenHeight;

    std::vector<float> depthBuffer;  // size = screenWidth
};

struct WallLightDataPerTile;

class Renderer {
public:
    Renderer(SDL_Window& window, int sWidth, int sHeight);
    ~Renderer();
private:
    SDL_Renderer* renderer;
    int screenWidth;
    int screenHeight;

    std::vector<std::vector<WallLightDataPerTile>> wallLightData; // size = MAP_WIDTH x MAP_HEIGHT

    float shadowDistanceFactor = 0.2f;
public:
    float GetShadowDistanceFactor() const { return shadowDistanceFactor; }
    void SetShadowDistanceFactor(float factor) { shadowDistanceFactor = factor; }

    void Render(const Level& level, const View& view);
    void RenderWalls(RenderFrameData& frame, const Level& level, const View& view);
};
