#include "srg/world/Cell.h"

#include "srg/world/Door.h"
#include "srg/world/Room.h"

#include <iostream>

namespace srg
{
namespace world
{
Cell::Cell(uint32_t x, uint32_t y)
        : ObjectSet()
        , coordinate(x, y)
        , up(nullptr)
        , down(nullptr)
        , left(nullptr)
        , right(nullptr)
        , room(nullptr)
{
}

RoomType Cell::getType() const
{
    return this->room->getType();
}

bool Cell::isBlocked() const
{
    if (this->getType() == srg::world::RoomType::Wall) {
        return true;
    }
    for (auto& objectEntry : this->containingObjects) {
        if (const Door* door = dynamic_cast<const Door*>(objectEntry.second)) {
            return !door->isOpen();
        }
    }
    return false;
}

bool Cell::operator<(const Cell* other)
{
    return this->coordinate < other->coordinate;
}

bool operator==(Cell const& first, Cell const& second)
{
    return first.coordinate == second.coordinate;
}

bool Cell::operator==(Cell const* other)
{
    return this->coordinate == other->coordinate;
}

bool operator!=(Cell const& first, Cell const& second)
{
    return !(first == second);
}

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
    os << "[Cell] " << cell.coordinate << " " << *(cell.room) << std::endl;
    os << "Contained Objects (Size " << cell.containingObjects.size() << "):" << std::endl;
    for (auto& objectEntry : cell.containingObjects) {
        os << *objectEntry.second << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg
