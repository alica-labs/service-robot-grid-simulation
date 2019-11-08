#pragma once

#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/RoomType.h"
#include "srgsim/world/Cell.h"

#include <map>

namespace srgsim
{
class Room
{
public:
    Room(std::string name, essentials::IdentifierConstPtr id);

    void addCell(Cell* cell);
    const Cell* getCell(const Coordinate& coord) const;
    RoomType getType() const;

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Room& obj)
    {
        os << "Room: " << obj.name << " ID: " << obj.id << " Type: " << obj.type;
        return os;
    }
    friend World;
private:
    essentials::IdentifierConstPtr id;
    RoomType type;
    std::string name;
    std::map<Coordinate, Cell*> cells;
};
} // namespace srgsim
