#pragma once

#include <iosfwd>

namespace srgsim
{
    enum class RoomType
    {
        Floor,
        Workroom,
        Bathroom,
        UtilityRoom,
        Kitchen,
        ReceptionRoom,
        ConferenceRoom,
        ServerRoom,
        Storeroom,
        WorkshopRoom,
        Wall,
    };

    std::ostream& operator<<(std::ostream& os, const RoomType& type);
}