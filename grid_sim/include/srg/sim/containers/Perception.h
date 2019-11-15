#pragma once

#include <srg/world/ObjectState.h>
#include <srg/world/ObjectType.h>

#include <essentials/IdentifierConstPtr.h>

#include <cstdint>

namespace srg
{
namespace sim
{
namespace containers
{
struct Perception
{
    srg::world::ObjectType type;
    srg::world::ObjectState state;
    essentials::IdentifierConstPtr objectID;
    essentials::IdentifierConstPtr robotID;
    uint32_t x;
    uint32_t y;

    friend std::ostream& operator<<(std::ostream& os, const Perception& obj)
    {
        os << "[Perception] Type: " << obj.state << " " << obj.type << "(" << (*obj.objectID) << ") at " << obj.x << ", " << obj.y
           << " Optional RobotID: " << (*obj.robotID) << std::endl;
        return os;
    }
};
} // namespace containers
} // namespace sim
} // namespace srg