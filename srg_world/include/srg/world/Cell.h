#pragma once

#include "srg/world/Coordinate.h"
#include "srg/world/RoomType.h"

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

class Cell
{
public:
    virtual ~Cell() = default;

    const std::vector<Object*>& getObjects() const;
    void addObject(Object* object);
    void removeObject(Object* object);
    void update(std::vector<Object*> objects);
    RoomType getType() const;
    bool isBlocked() const;
    bool contains(const Object* object) const;
    bool contains(essentials::IdentifierConstPtr objectID) const;

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

    std::vector<Object*> objects;
};

bool operator==(const Cell& first, const Cell& second);
bool operator!=(const Cell& first, const Cell& second);
bool operator<(const Cell& first, const Cell& second);

} // namespace world
} // namespace srg