// teapot/grid/Grid3D.h
#pragma once
#include <vector>
#include <optional>
#include <cassert>

template <typename T = int>
class Grid3D {
public:
    Grid3D(int width, int height, int depth, T defaultValue = T{})
        : width(width), height(height), depth(depth),
          cells(width * height * depth, defaultValue) {
        assert(width > 0 && height > 0 && depth > 0);
    }

    int Width()  const { return width; }
    int Height() const { return height; }
    int Depth()  const { return depth; }
    int Size()   const { return width * height * depth; }

    bool InBounds(int x, int y, int z) const {
        return x >= 0 && x < width
            && y >= 0 && y < height
            && z >= 0 && z < depth;
    }

    std::optional<T> Get(int x, int y, int z) const {
        if (!InBounds(x, y, z)) return std::nullopt;
        return cells[Index(x, y, z)];
    }

    bool Set(int x, int y, int z, T value) {
        if (!InBounds(x, y, z)) return false;
        cells[Index(x, y, z)] = value;
        return true;
    }

    void Fill(T value) {
        std::fill(cells.begin(), cells.end(), value);
    }

    // Slice a full 2D layer at the given z depth
    std::optional<Grid2D<T>> Slice(int z) const {
        if (z < 0 || z >= depth) return std::nullopt;

        Grid2D<T> layer(width, height);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                layer.Set(x, y, cells[Index(x, y, z)]);
            }
        }
        return layer;
    }

    const T* Data() const { return cells.data(); }
    T*       Data()       { return cells.data(); }

private:
    int Index(int x, int y, int z) const {
        return z * (width * height) + y * width + x;
    }

    int width;
    int height;
    int depth;
    std::vector<T> cells;
};