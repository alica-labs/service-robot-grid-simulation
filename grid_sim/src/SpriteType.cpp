#include "srgsim/SpriteType.h"

#include <iostream>

namespace srgsim
{

/**
 * For getting a string representation of a SpriteObjectType.
 * @param os Outputstream
 * @param type Type of a sprite or object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const SpriteType& type)
{
    switch (type) {
    case SpriteType::Default:
        os << "Default";
        break;
    case SpriteType::Gras:
        os << "Gras";
        break;
    case SpriteType::Wall:
        os << "Wall";
        break;
    case SpriteType::DoorOpen:
        os << "DoorOpen";
        break;
    case SpriteType::Black:
        os << "Black";
        break;
    case SpriteType::Unknown:
        os << "Unknown";
        break;
    case SpriteType::Floor:
        os << "Floor";
        break;
    case SpriteType::DoorClosed:
        os << "DoorClosed";
        break;
    case SpriteType::Robot:
        os << "Robot";
        break;
    case SpriteType::White:
        os << "White";
        break;
    case SpriteType::Dirt:
        os << "Dirt";
        break;
    case SpriteType::White2:
        os << "White2";
        break;
    case SpriteType::CupRed:
        os << "CupRed";
        break;
    case SpriteType::CupBlue:
        os << "CupBlue";
        break;
    case SpriteType::CupYellow:
        os << "CupYellow";
        break;
    case SpriteType::Last:
        os << "Last";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace srgsim