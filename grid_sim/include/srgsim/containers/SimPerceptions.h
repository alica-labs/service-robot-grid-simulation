#pragma once

#include "Perception.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srgsim
{
    struct SimPerceptions
    {
        essentials::IdentifierConstPtr receiverID;
        std::vector<Perception> perceptions;
    };
} // namespace srgsim