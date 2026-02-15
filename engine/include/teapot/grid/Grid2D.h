#pragma once
#include <vector>
#include <optional>
#include <cassert>

template <typename T = int>
class Grid2D {
public:
    Grid2D(int width, int height, T defaultValue = T{})
        : width(width), height(height),
          cells(width * height, defaultValue) {
        assert(width > 0 && height > 0);
    }

    int Width()  const { return width; }
    int Height() const { return height; }
    int Size()   const { return width * height; }

    bool InBounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    std::optional<T> Get(int x, int y) const {
        if (!InBounds(x, y)) return std::nullopt;
        return cells[Index(x, y)];
    }

    bool Set(int x, int y, T value) {
        if (!InBounds(x, y)) return false;
        cells[Index(x, y)] = value;
        return true;
    }

    void Fill(T value) {
        std::fill(cells.begin(), cells.end(), value);
    }

    const T* Data() const { return cells.data(); }
    T*       Data()       { return cells.data(); }

private:
    int Index(int x, int y) const {
        return y * width + x;
    }

    int width;
    int height;
    std::vector<T> cells;
};