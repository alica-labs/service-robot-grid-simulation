#include "srg/world/ObjectType.h"

#include <iostream>

namespace srg
{
namespace world
{

/**
 * For getting a string representation of a SpriteObjectType.
 * @param os Outputstream
 * @param type Type of a sprite or object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const ObjectType& type)
{
    switch (type) {
    case ObjectType::Door:
        os << "Door";
        break;
    case ObjectType::CupRed:
        os << "CupRed";
        break;
    case ObjectType::CupYellow:
        os << "CupYellow";
        break;
    case ObjectType::CupBlue:
        os << "CupBlue";
        break;
    case ObjectType::Robot:
        os << "Robot";
        break;
    case ObjectType::Human:
        os << "Human";
        break;
    case ObjectType::Unknown:
        os << "Unknown";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace world
} // namespace srg