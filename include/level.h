#pragma once

#include <vector>

constexpr int MAP_WIDTH = 256;
constexpr int MAP_HEIGHT = 256;

using MapArray = std::vector<std::vector<int>>;

struct LightData {
    float Intensity = 0.0f;
    float R = 0.0f;
    float G = 0.0f;
    float B = 0.0f;
};

constexpr int WALL_SHADING_SUBDIVISIONS = 4;

struct WallLightData {
    LightData[WALL_SHADING_SUBDIVISIONS] Subdivisions;
};

const enum WallFaces = {
    North = 0,
    South = 1,
    East = 2,
    West = 3
};

struct WallLightDataPerFace {
    WallLightData Faces[4];
};

class Level {
public:
    Level();
private:
    MapArray map = MapArray(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, 0));
    int SolidMask = 0;

public:
    int GetTile(const int x, const int y) const {
        if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 0;
        return map[x][y];
    }

    void AddSolid(const int layer) { SolidMask |= 1 << layer; }
    void RemoveSolid(const int layer) { SolidMask &= ~(1 << layer); }
    void ClearSolids() { SolidMask = 0; }

    void SetMap(const MapArray& newMap);

    bool IsWall(const int x, const int y) const;
};