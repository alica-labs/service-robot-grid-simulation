#pragma once

#include <iosfwd>

namespace srg
{
namespace world
{
enum class RoomType
{
    Floor,//
    Office,
    Bathroom,//
    UtilityRoom,
    Kitchen,//
    ReceptionRoom,
    ConferenceRoom,
    ServerRoom,
    Storeroom,
    WorkshopRoom,
    Wall,
};

std::ostream& operator<<(std::ostream& os, const RoomType& type);
} // namespace world
} // namespace srg