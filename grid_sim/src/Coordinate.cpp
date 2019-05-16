#include "container/Coordinate.h"
#include <iostream>

namespace grid_sim
{
Coordinate::Coordinate(uint32_t x, uint32_t y)
{
    this->x = x;
    this->y = y;
}

Coordinate::Coordinate(const grid_sim::Coordinate& coordinate)
{
    this->x = coordinate.x;
    this->y = coordinate.y;
}
bool operator==(Coordinate const& first, Coordinate const& second)
{
    std::cout << "Coordinate: == called" << std::endl;
    return first.x == second.x && first.y == second.y;
}

} // namespace grid_sim