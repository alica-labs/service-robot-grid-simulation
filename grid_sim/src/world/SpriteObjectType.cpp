#include "srgsim/world/SpriteObjectType.h"

#include <iostream>

namespace srgsim
{

/**
 * For getting a string representation of a SpriteObjectType.
 * @param os Outputstream
 * @param type Type of a sprite or object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const SpriteObjectType& type)
{
    switch (type) {
    case SpriteObjectType::Dirt:
        os << "Dirt";
        break;
    case SpriteObjectType::Gras:
        os << "Gras";
        break;
    case SpriteObjectType::Wall:
        os << "Wall";
        break;
    case SpriteObjectType::DoorOpen:
        os << "DoorOpen";
        break;
    case SpriteObjectType::Black:
        os << "Black";
        break;
    case SpriteObjectType::Unknown:
        os << "Unknown";
        break;
    case SpriteObjectType::Floor:
        os << "Floor";
        break;
    case SpriteObjectType::DoorClosed:
        os << "DoorClosed";
        break;
    case SpriteObjectType::Robot:
        os << "Robot";
        break;
    case SpriteObjectType::White:
        os << "White";
        break;
    case SpriteObjectType::Default:
        os << "Default";
        break;
    case SpriteObjectType::White2:
        os << "White2";
        break;
    case SpriteObjectType::CupRed:
        os << "CupRed";
        break;
    case SpriteObjectType::CupBlue:
        os << "CupBlue";
        break;
    case SpriteObjectType::CupYellow:
        os << "CupYellow";
        break;
    case SpriteObjectType::Door:
        os << "Door";
        break;
    case SpriteObjectType::Last:
        os << "Last";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace srgsim