#pragma once

#include <iosfwd>
#include <string>

namespace srg
{
namespace world
{
enum class RoomType
{
    Floor, //
    Office,
    Bathroom, //
    UtilityRoom,
    Kitchen, //
    ReceptionRoom,
    ConferenceRoom,
    ServerRoom,
    Storeroom,
    WorkshopRoom,
    Wall,
    Unknown
};

static RoomType FromString(const std::string& typeAsString)
{
    if (typeAsString.compare("floor") == 0) {
        return RoomType::Floor;
    } else if (typeAsString.compare("office") == 0) {
        return RoomType::Office;
    } else if (typeAsString.compare("bathroom") == 0) {
        return RoomType::Bathroom;
    } else if (typeAsString.compare("utility_room") == 0) {
        return RoomType::UtilityRoom;
    } else if (typeAsString.compare("kitchen") == 0) {
        return RoomType::Kitchen;
    } else if (typeAsString.compare("reception_room") == 0) {
        return RoomType::ReceptionRoom;
    } else if (typeAsString.compare("conference_room") == 0) {
        return RoomType::ConferenceRoom;
    } else if (typeAsString.compare("server_room") == 0) {
        return RoomType::ServerRoom;
    } else if (typeAsString.compare("storeroom") == 0) {
        return RoomType::Storeroom;
    } else if (typeAsString.compare("workshop_room") == 0) {
        return RoomType::WorkshopRoom;
    } else if (typeAsString.compare("wall") == 0) {
        return RoomType::Wall;
    }
    return RoomType::Unknown;
}

std::ostream& operator<<(std::ostream& os, const RoomType& type);
} // namespace world
} // namespace srg