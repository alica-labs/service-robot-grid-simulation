#pragma once

#include <iosfwd>

namespace srgsim
{
enum class ObjectState
{
    Open,
    Closed,
    Undefined
};

std::ostream& operator<<(std::ostream& os, const ObjectState& objectState);
}