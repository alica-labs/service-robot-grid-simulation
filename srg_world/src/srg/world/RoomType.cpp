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
        os << "conferenceRoom";
        break;
    case RoomType::Kitchen:
        os << "kitchen";
        break;
    case RoomType::ReceptionRoom:
        os << "receptionRoom";
        break;
    case RoomType::ServerRoom:
        os << "serverRoom";
        break;
    case RoomType::Storeroom:
        os << "storeroom";
        break;
    case RoomType::UtilityRoom:
        os << "utilityRoom";
        break;
    case RoomType::Workroom:
        os << "workroom";
        break;
    case RoomType::WorkshopRoom:
        os << "workshopRoom";
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