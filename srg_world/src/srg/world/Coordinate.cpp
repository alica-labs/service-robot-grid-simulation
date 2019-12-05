#include "srg/world/Coordinate.h"

#include <iostream>

namespace srg
{
namespace world
{
Coordinate::Coordinate(int32_t x, int32_t y)
{
    this->x = x;
    this->y = y;
}

Coordinate::~Coordinate() {}

Coordinate::Coordinate(const Coordinate& coordinate)
{
    this->x = coordinate.x;
    this->y = coordinate.y;
}

Coordinate Coordinate::abs()
{
    return Coordinate(std::abs(x), std::abs(y));
}

bool operator==(Coordinate const& first, Coordinate const& second)
{
    return first.x == second.x && first.y == second.y;
}

bool operator!=(Coordinate const& first, Coordinate const& second)
{
    return !(first == second);
}

bool operator<(Coordinate const& first, Coordinate const& second)
{
    if (first.x < second.x) {
        return true;
    } else if (first.x > second.x) {
        return false;
    } else {
        return (first.y < second.y);
    }
}

Coordinate operator-(Coordinate const& first, Coordinate const& second)
{
    return Coordinate(first.x - second.x, first.y - second.y);
}

std::ostream& operator<<(std::ostream& os, const Coordinate& obj)
{
    os << obj.x << "," << obj.y;
    return os;
}
} // namespace world
} // namespace srg