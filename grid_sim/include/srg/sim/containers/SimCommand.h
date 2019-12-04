#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srg
{
namespace sim
{
namespace containers
{
struct SimCommand
{
    enum Action
    {
        SPAWNROBOT,
        SPAWNHUMAN,
        GOLEFT,
        GORIGHT,
        GOUP,
        GODOWN,
        PICKUP,
        PUTDOWN,
        OPEN,
        CLOSE
    };

    essentials::IdentifierConstPtr senderID;
    Action action;
    essentials::IdentifierConstPtr objectID;
    uint32_t x;
    uint32_t y;
};
} // namespace containers
} // namespace sim
} // namespace srg