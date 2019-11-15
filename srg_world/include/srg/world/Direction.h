#pragma once

#include <iosfwd>

namespace srg
{
namespace world
{
enum class Direction
{
    Left,
    Up,
    Down,
    Right,
    None
};

std::ostream& operator<<(std::ostream& os, const Direction& direction);
} // namespace world
} // namespace srg