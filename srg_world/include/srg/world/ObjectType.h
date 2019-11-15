#pragma once

#include <iosfwd>

namespace srg
{
namespace world
{
enum class ObjectType
{
    Door,
    CupRed,
    CupBlue,
    CupYellow,
    Robot,
    Unknown,
};

std::ostream& operator<<(std::ostream& os, const ObjectType& type);
} // namespace world
} // namespace srg