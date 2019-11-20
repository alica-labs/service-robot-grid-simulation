#pragma once

#include <iosfwd>

namespace srg
{
namespace world
{
enum class ObjectState
{
    Open,
    Closed,
    Undefined
};

std::ostream& operator<<(std::ostream& os, const ObjectState& objectState);
} // namespace world
} // namespace srg