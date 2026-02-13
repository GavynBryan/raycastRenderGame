#include "level.h"

Level::Level()
{
    for(int x = 0; x < MAP_WIDTH; x++) {
        for(int y = 0; y < MAP_HEIGHT; y++) {
            map[x][y] = 0;
        }
    }
}

void Level::SetMap(const MapArray& newMap)
{
    for (int x = 0; x < MAP_WIDTH; x++)
        for (int y = 0; y < MAP_HEIGHT; y++)
            map[x][y] = 0;

    for (size_t x = 0; x < newMap.size() && x < MAP_WIDTH; x++)
        for (size_t y = 0; y < newMap[x].size() && y < MAP_HEIGHT; y++)
            map[x][y] = newMap[x][y];
}

bool Level::IsWall(const int x, const int y) const {
    if(x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return true;
    return map[x][y] & SolidMask;
}

