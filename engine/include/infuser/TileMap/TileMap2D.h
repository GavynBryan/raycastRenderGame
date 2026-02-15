// infuser/tilemap/TileMap2D.h
#pragma once
#include <teapot/grid/Grid2D.h>
#include <teapot/math/Vec.h>

template <typename T = int>
class TileMap2D {
public:
    TileMap2D(int width, int height, Vec2 origin = {0, 0}, T emptyTile = T{})
        : grid(width, height, emptyTile),
          origin(origin),
          emptyTile(emptyTile) {}

    // Local-space (grid coordinates)
    std::optional<T> GetLocal(int x, int y) const {
        return grid.Get(x, y);
    }

    bool SetLocal(int x, int y, T value) {
        return grid.Set(x, y, value);
    }

    // World-space
    std::optional<T> GetWorld(int x, int y) const {
        return grid.Get(ToLocalX(x), ToLocalY(y));
    }

    bool SetWorld(int x, int y, T value) {
        return grid.Set(ToLocalX(x), ToLocalY(y));
    }

    T GetWorldOr(int x, int y) const {
        return GetWorld(x, y).value_or(emptyTile);
    }

    T GetLocalOr(int x, int y) const {
        return GetLocal(x, y).value_or(emptyTile);
    }

    const Grid2D<T>&        Grid()      const { return grid; }
    Vec2                    Origin()    const { return origin; }
    T                       EmptyTile() const { return emptyTile; }

private:
    int ToLocalX(int worldX) const { return worldX - static_cast<int>(origin.X); }
    int ToLocalY(int worldY) const { return worldY - static_cast<int>(origin.Y); }

    Grid2D<T>               grid;
    Vec2                    origin;
    T                       emptyTile;
};