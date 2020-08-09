#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>
#include <chrono>

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

    std::chrono::system_clock::duration timestamp;
    essentials::IdentifierConstPtr senderID;
    Action action;
    essentials::IdentifierConstPtr objectID;
    uint32_t x;
    uint32_t y;
};
} // namespace containers
} // namespace sim
} // namespace srg