#include "Cell.h"
#include <iostream>

namespace srgsim
{
Cell::Cell(uint32_t x, uint32_t y)
        : coordinate(x, y)
{
    this->up = nullptr;
    this->down = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->type = Type::Unknown;

}

Cell::Cell(const srgsim::Cell& cell)
        : coordinate(cell.coordinate)
{
    this->room = cell.room;
    this->type = cell.type;
    this->up = cell.up;
    this->down = cell.down;
    this->left = cell.left;
    this->right = cell.right;
 }
} // namespace srgsim
