#pragma once

struct LightData {
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

constexpr int WALL_SHADING_SUBDIVISIONS = 4;

struct WallLightData {
    LightData subdivisions[WALL_SHADING_SUBDIVISIONS];
};

enum WallFace {
    North = 0,
    South,
    East,
    West
};

struct WallLightDataPerTile {
    WallLightData faces[4];
};
