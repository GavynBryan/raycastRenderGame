#include "level.h"

Level::Level()
    :tileMap(MAP_DEFAULT_WIDTH * MAP_DEFAULT_HEIGHT),
    width(MAP_DEFAULT_WIDTH),
    height(MAP_DEFAULT_HEIGHT)
{
}

Level::Level(int width, int height)
    :tileMap(width * height),
    width(width),
    height(height)
{
}

void Level::SetMap(const MapArray& newMap)
{
    for (int x = 0; x < width; x++)
        for (int y = 0; y < height; y++)
            tileMap[GetIndex(x, y)] = 0;

    for (size_t x = 0; x < newMap.size() && x < width; x++)
        for (size_t y = 0; y < newMap[x].size() && y < height; y++)
            tileMap[GetIndex(x, y)] = newMap[x][y];
}

bool Level::IsWall(const int x, const int y) const {
    if(x < 0 || x >= width || y < 0 || y >= height) return true;
    return tileMap[GetIndex(x, y)] & SolidMask;
}

