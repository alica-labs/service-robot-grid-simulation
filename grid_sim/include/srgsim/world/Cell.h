#pragma once

#include "srgsim/containers/Coordinate.h"
#include "srgsim/containers/CellPerceptions.h"

#include "TaskType.h"
#include "RoomType.h"

#include <string>
#include <vector>
#include <iostream>

namespace srgsim
{
class World;
class Object;
class Room;

class Cell
{
public:
    friend World;
    virtual ~Cell() = default;

    const std::vector<Object *> &getObjects() const;
    void addObject(Object* object);
    void removeObject(Object* object);
    void update(std::vector<Object*> objects);
    RoomType getType() const;
    friend std::ostream& operator<<(std::ostream& os, const srgsim::Cell& obj);
    bool operator<(const Cell* other);
    bool operator==(Cell const* other);
    static bool sortByCoordinates(const Cell* first, const Cell* second);

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

bool operator==(const Cell& first,const Cell& second);
bool operator!=(const Cell& first,const Cell& second);
bool operator<(const Cell& first,const Cell& second);

} // namespace srgsim