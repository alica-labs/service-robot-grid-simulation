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
        break;
    case Action::SPAWNHUMAN:
        os << "SPAWNHUMAN";
            break;
    case Action::GOLEFT:
        os << "GOLEFT";
            break;
    case Action::GORIGHT:
        os << "GORIGHT";
            break;
    case Action::GOUP:
        os << "GOUP";
            break;
    case Action::GODOWN:
        os << "GODOWN";
            break;
    case Action::PICKUP:
        os << "PICKUP";
            break;
    case Action::PUTDOWN:
        os << "PUTDOWN";
            break;
    case Action::OPEN:
        os << "OPEN";
            break;
    case Action::CLOSE:
        os << "CLOSE";
            break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace containers
} // namespace sim
} // namespace srg
