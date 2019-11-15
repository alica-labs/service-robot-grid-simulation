#include "srg/world/Room.h"

#include "srg/world/Cell.h"

namespace srg
{
namespace world
{
Room::Room(std::string name, essentials::IdentifierConstPtr id)
        : name(name)
        , id(id)
{
}

RoomType Room::getType() const
{
    return this->type;
}

void Room::addCell(Cell* cell)
{
    cell->room = this;

    auto cellEntry = this->cells.find(cell->coordinate);
    if (cellEntry == this->cells.end()) {
        this->cells.emplace(cell->coordinate, cell);
    }
}

const Cell* Room::getCell(const Coordinate& coordinate) const
{
    if (this->cells.find(coordinate) != this->cells.end()) {
        return this->cells.at(coordinate);
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Room& obj)
{
    os << "[Room] " << obj.name << " ID: " << obj.id << " Type: " << obj.type;
    return os;
}
} // namespace world
} // namespace srg