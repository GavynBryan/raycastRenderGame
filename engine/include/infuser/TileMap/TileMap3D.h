// infuser/tilemap/TileMap3D.h
#pragma once
#include <teapot/grid/Grid3D.h>
#include <teapot/math/Vec.h>

template <typename T = int>
class TileMap3D {
public:
    TileMap3D(int width, int height, int depth, Vec3 origin = {0, 0, 0}, T emptyTile = T{})
        : grid(width, height, depth, emptyTile),
          origin(origin),
          emptyTile(emptyTile) {}

    // Local-space (grid coordinates)
    std::optional<T> GetLocal(int x, int y, int z) const {
        return grid.Get(x, y, z);
    }

    bool SetLocal(int x, int y, int z, T value) {
        return grid.Set(x, y, z, value);
    }

    // World-space
    std::optional<T> GetWorld(int x, int y, int z) const {
        return grid.Get(ToLocalX(x), ToLocalY(y), ToLocalZ(z));
    }

    bool SetWorld(int x, int y, int z, T value) {
        return grid.Set(ToLocalX(x), ToLocalY(y), ToLocalZ(z));
    }

    T GetWorldOr(int x, int y, int z) const {
        return GetWorld(x, y, z).value_or(emptyTile);
    }

    T GetLocalOr(int x, int y, int z) const {
        return GetLocal(x, y, z).value_or(emptyTile);
    }

    // Slice a full 2D layer at local z, returned as a standalone TileMap2D
    std::optional<TileMap2D<T>> SliceLocal(int z) const {
        auto layer = grid.Slice(z);
        if (!layer) return std::nullopt;

        Vec2 origin2D = {origin.X, origin.Y};
        TileMap2D<T> map(layer->Width(), layer->Height(), origin2D, emptyTile);
        for (int y = 0; y < layer->Height(); ++y) {
            for (int x = 0; x < layer->Width(); ++x) {
                auto val = layer->Get(x, y);
                if (val) map.SetLocal(x, y, *val);
            }
        }
        return map;
    }

    // Slice at a world z coordinate
    std::optional<TileMap2D<T>> SliceWorld(int z) const {
        return SliceLocal(ToLocalZ(z));
    }

    const Grid3D<T>&            Grid()      const { return grid; }
    Vec3                        Origin()    const { return origin; }
    T                           EmptyTile() const { return emptyTile; }

private:
    int ToLocalX(int worldX) const { return worldX - static_cast<int>(origin.X); }
    int ToLocalY(int worldY) const { return worldY - static_cast<int>(origin.Y); }
    int ToLocalZ(int worldZ) const { return worldZ - static_cast<int>(origin.Z); }

    Grid3D<T>                   grid;
    Vec3                        origin;
    T                           emptyTile;
};