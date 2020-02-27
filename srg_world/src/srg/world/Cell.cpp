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
        if (std::shared_ptr<const world::Door> door = std::dynamic_pointer_cast<const Door>(objectEntry.second)) {
            return !door->isOpen();
        }
    }
    return false;
}

bool Cell::operator<(std::shared_ptr<const Cell> other)

{
    return this->coordinate < other->coordinate;
}

bool operator==(std::shared_ptr<const Cell> first, std::shared_ptr<const Cell> second)
{
    return first->coordinate == second->coordinate;
}

bool Cell::operator==(std::shared_ptr<const Cell> other)
{
    return this->coordinate == other->coordinate;
}

bool operator!=(std::shared_ptr<const Cell> first, std::shared_ptr<const Cell> second)
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
