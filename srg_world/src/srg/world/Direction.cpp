#include "srg/world/Direction.h"

#include <iostream>

namespace srg
{
namespace world
{
/**
 * For getting a string representation of an direction.
 * @param os Outputstream
 * @param type the direction.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const Direction& direction)
{
    switch (direction) {
    case Direction::None:
        os << "None";
        break;
    case Direction::Left:
        os << "Left";
        break;
    case Direction::Right:
        os << "Right";
        break;
    case Direction::Up:
        os << "Up";
        break;
    case Direction::Down:
        os << "Down";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace world
} // namespace srg