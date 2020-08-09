#pragma once

#include "CellPerception.h"

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>
#include <chrono>

namespace srg
{
namespace sim
{
namespace containers
{
struct Perceptions
{
    std::chrono::system_clock::duration timestamp;
    essentials::IdentifierConstPtr receiverID;
    std::vector<CellPerception> cellPerceptions;
};
} // namespace containers
} // namespace sim
} // namespace srg