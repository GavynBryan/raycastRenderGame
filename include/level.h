#pragma once

#include <vector>

constexpr int MAP_DEFAULT_WIDTH = 256;
constexpr int MAP_DEFAULT_HEIGHT = 256;

using MapArray = std::vector<std::vector<int>>;

class Level {
public:
    Level();
    Level(int width, int height);
private:
    std::vector<int> tileMap;
    int SolidMask = 0;

    int width;
    int height;

public:

    int GetIndex(const int x, const int y) const {
        return y * width + x;
    }

    int GetTile(const int x, const int y) const {
        if(x < 0 || x >= width || y < 0 || y >= height) return 0;
        return tileMap[GetIndex(x, y)];
    }

    void AddSolid(const int layer) { SolidMask |= 1 << layer; }
    void RemoveSolid(const int layer) { SolidMask &= ~(1 << layer); }
    void ClearSolids() { SolidMask = 0; }

    void SetMap(const MapArray& newMap);

    bool IsWall(const int x, const int y) const;
};