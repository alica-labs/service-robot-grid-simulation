#pragma once

#include "container/Coordinate.h"

#include "SRGEnums.h"

#include <string>

namespace srgsim
{
class World;

class Cell
{
public:
    friend World;
    virtual ~Cell() = default;
    Cell(const Cell& cell);

    Type type;
    std::string room;
    Coordinate coordinate;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;

private:
    Cell(uint32_t x, uint32_t y);
};

} // namespace srgsim