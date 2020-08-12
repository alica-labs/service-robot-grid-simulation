#pragma once

#include <iosfwd>

namespace srg
{
namespace sim
{
namespace containers
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
std::ostream& operator<<(std::ostream& os, const Action& direction);
}
} // namespace sim
} // namespace srg
