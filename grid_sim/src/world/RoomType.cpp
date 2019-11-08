#include "srgsim/world/RoomType.h"

#include <iostream>

namespace srgsim
{

/**
 * For getting a string representation of a SpriteObjectType.
 * @param os Outputstream
 * @param type Type of a sprite or object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const RoomType& type)
{
    switch (type) {
    case RoomType::Floor:
        os << "Floor";
        break;
    case RoomType::Bathroom:
        os << "Bathroom";
        break;
    case RoomType::ConferenceRoom:
        os << "ConferenceRoom";
        break;
    case RoomType::Kitchen:
        os << "Kitchen";
        break;
    case RoomType::ReceptionRoom:
        os << "ReceptionRoom";
        break;
    case RoomType::ServerRoom:
        os << "ServerRoom";
        break;
    case RoomType::Storeroom:
        os << "Storeroom";
        break;
    case RoomType::UtilityRoom:
        os << "UtilityRoom";
        break;
    case RoomType::Workroom:
        os << "Workroom";
        break;
    case RoomType::WorkshopRoom:
        os << "WorkshopRoom";
        break;
    case RoomType::Wall:
        os << "Wall";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace srgsim