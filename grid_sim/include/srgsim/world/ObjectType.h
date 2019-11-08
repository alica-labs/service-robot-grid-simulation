#pragma once

#include <iosfwd>

namespace srgsim
{
    enum class ObjectType
    {
        Door,
        CupRed,
        CupBlue,
        CupYellow,
        Robot,
    };

    std::ostream& operator<<(std::ostream& os, const ObjectType& type);
}