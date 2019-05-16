#pragma once

#include "container/Coordinate.h"

#include <string>

namespace grid_sim {

enum Type{
    Wall, Floor, Door
};

class Cell {
public:
    Cell(uint32_t x, uint32_t y);
    virtual ~Cell() = default;
    Cell(const Cell& cell);

    Type type;
    std::string room;
    Coordinate coordinate;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;
};

}