#pragma once

#include "srg/world/Cell.h"
#include "srg/world/Coordinate.h"
#include "srg/world/RoomType.h"

#include <essentials/IdentifierConstPtr.h>

#include <map>

namespace srg
{
class World;
namespace world
{

class Room
{
public:
    Room(std::string name, essentials::IdentifierConstPtr id);
    void addCell(Cell* cell);
    const Cell* getCell(const Coordinate& coord) const;
    RoomType getType() const;

    // friend declaration
    friend std::ostream& operator<<(std::ostream& os, const Room& obj);
    friend ::srg::World;

private:
    essentials::IdentifierConstPtr id;
    RoomType type;
    std::string name;
    std::map<Coordinate, Cell*> cells;
};
} // namespace world
} // namespace srg
