#pragma once

#include "srg/world/Coordinate.h"
#include "srg/world/RoomType.h"
#include "srg/world/ObjectSet.h"

#include <essentials/IdentifierConstPtr.h>

#include <iosfwd>
#include <string>
#include <vector>

namespace srg
{
class World;
namespace world
{
class Object;
class Room;

class Cell : public ObjectSet
{
public:
    RoomType getType() const;
    bool isBlocked() const;

    bool operator<(const Cell* other);
    bool operator==(Cell const* other);

    // friend declaration
    friend ::srg::World;
    friend std::ostream& operator<<(std::ostream& os, const Cell& obj);

    Coordinate coordinate;
    Room* room;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;

private:
    Cell(uint32_t x, uint32_t y);
};

bool operator==(const Cell& first, const Cell& second);
bool operator!=(const Cell& first, const Cell& second);
bool operator<(const Cell& first, const Cell& second);

} // namespace world
} // namespace srg