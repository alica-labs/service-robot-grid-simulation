#pragma once

#include "Action.h"

#include <essentials/IdentifierConstPtr.h>

#include <chrono>
#include <cstdint>

namespace srg
{
namespace sim
{
namespace containers
{
struct SimCommand
{
    std::chrono::system_clock::duration timestamp;
    essentials::IdentifierConstPtr senderID;
    Action action;
    essentials::IdentifierConstPtr objectID;
    uint32_t x;
    uint32_t y;

    friend std::ostream& operator<<(std::ostream& os, const SimCommand& obj)
    {
        os << "[SimCommand] Sender " << obj.senderID << " wants to " << obj.action << " Coordinates: (" << obj.x << ", " << obj.y << ")"
           << " ObjectID: " << obj.objectID << std::endl;
        return os;
    }
};
} // namespace containers
} // namespace sim
} // namespace srg