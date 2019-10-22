#pragma once

#include "srgsim/SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srgsim
{
    struct Perception
    {
        Type type;
        essentials::IdentifierConstPtr objectID;
        uint32_t x;
        uint32_t y;

        friend std::ostream& operator<<(std::ostream& os, const srgsim::Perception& obj)
        {
            os << "Perception - Type: " << obj.type << " ID: " << (*obj.objectID) << " At: (" << obj.x << ", " << obj.y << ")";
            return os;
        }
    };
} // namespace srgsim