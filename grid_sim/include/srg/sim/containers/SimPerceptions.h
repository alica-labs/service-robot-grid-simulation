#pragma once

#include "CellPerception.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srg
{
namespace sim
{
namespace containers
{
struct SimPerceptions
{
    essentials::IdentifierConstPtr receiverID;
    std::vector<CellPerception> cellPerceptions;
};
} // namespace containers
} // namespace sim
} // namespace srg