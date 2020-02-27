#pragma once

#include "srg/world/Coordinate.h"
#include "srg/world/RoomType.h"
#include "srg/world/ObjectSet.h"

#include <essentials/IdentifierConstPtr.h>

#include <iosfwd>
#include <string>
#include <vector>
#include <memory>

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

    bool operator<(std::shared_ptr<const Cell> other);
    bool operator==(std::shared_ptr<const Cell> other);

    // friend declaration
    friend ::srg::World;
    friend std::ostream& operator<<(std::ostream& os, const Cell& obj);

    Coordinate coordinate;
    Room* room;
    std::shared_ptr<Cell> up;
    std::shared_ptr<Cell> down;
    std::shared_ptr<Cell> left;
    std::shared_ptr<Cell> right;
    int64_t timeOfLastUpdate;

private:
    Cell(uint32_t x, uint32_t y);
};

bool operator==(std::shared_ptr<const Cell> first, std::shared_ptr<const Cell> second);
bool operator!=(std::shared_ptr<const Cell> first, std::shared_ptr<const Cell> second);
//bool operator<(const Cell& first, const Cell& second);

} // namespace world
} // namespace srg