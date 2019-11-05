#pragma once

#include <iosfwd>

namespace srgsim
{
enum class Direction
{
    Left,
    Up,
    Down,
    Right,
    None
};

std::ostream& operator<<(std::ostream& os, const Direction & direction);
}