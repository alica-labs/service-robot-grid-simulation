#include "srg/world/RoomType.h"

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
std::ostream& operator<<(std::ostream& os, const RoomType& type)
{
    switch (type) {
    case RoomType::Floor:
        os << "floor";
        break;
    case RoomType::Bathroom:
        os << "bathroom";
        break;
    case RoomType::ConferenceRoom:
        os << "conference_room";
        break;
    case RoomType::Kitchen:
        os << "kitchen";
        break;
    case RoomType::ReceptionRoom:
        os << "reception_room";
        break;
    case RoomType::ServerRoom:
        os << "server_room";
        break;
    case RoomType::Storeroom:
        os << "storeroom";
        break;
    case RoomType::UtilityRoom:
        os << "utility_room";
        break;
    case RoomType::Office:
        os << "office";
        break;
    case RoomType::WorkshopRoom:
        os << "workshop_room";
        break;
    case RoomType::Wall:
        os << "wall";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}

} // namespace world
} // namespace srg