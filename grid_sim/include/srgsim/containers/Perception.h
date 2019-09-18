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
    };
} // namespace srgsim