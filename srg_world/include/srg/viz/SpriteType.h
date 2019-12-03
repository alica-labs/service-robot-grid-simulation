#pragma once

#include <iosfwd>

namespace srg
{
namespace viz
{
enum class SpriteType
{
    Default = 0,
    Gras = 1,
    Wall = 2,
    DoorOpen = 3,
    Black = 4,
    Unknown = 5,
    Floor = 6,
    DoorClosed = 7,
    Robot = 8,
    White = 9,
    Dirt = 10,
    White2 = 11,
    CupRed = 12,
    CupBlue = 13,
    CupYellow = 14,
    Human = 15,
    Last // has to be last for iterating with ints over this enum
};

std::ostream& operator<<(std::ostream& os, const SpriteType& type);
} // namespace viz
} // namespace srg