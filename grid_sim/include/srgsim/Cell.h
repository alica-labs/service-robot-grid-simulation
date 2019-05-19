#pragma once

#include "container/Coordinate.h"

#include "SRGEnums.h"

#include <string>
#include <vector>

namespace srgsim
{
class World;
class Object;

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
    std::vector<Object*> objects;

private:
    Cell(uint32_t x, uint32_t y);
};

} // namespace srgsim