#include "srg/sim/containers/Action.h"

#include <iostream>

namespace srg
{
namespace sim
{
namespace containers
{
std::ostream& operator<<(std::ostream& os, const Action& direction)
{
    switch (direction) {
    case Action::SPAWNROBOT:
        os << "SPAWNROBOT";
    case Action::SPAWNHUMAN:
        os << "SPAWNHUMAN";
    case Action::GOLEFT:
        os << "GOLEFT";
    case Action::GORIGHT:
        os << "GORIGHT";
    case Action::GOUP:
        os << "GOUP";
    case Action::GODOWN:
        os << "GODOWN";
    case Action::PICKUP:
        os << "PICKUP";
    case Action::PUTDOWN:
        os << "PUTDOWN";
    case Action::OPEN:
        os << "OPEN";
    case Action::CLOSE:
        os << "CLOSE";
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace containers
} // namespace sim
} // namespace srg
