#pragma once

#include "container/Coordinate.h"

#include "SRGEnums.h"

#include <string>
#include <vector>

namespace srgsim
{
class World;
class Object;

class Cell
{
public:
    friend World;
    virtual ~Cell() = default;
    Cell(const Cell& cell);

    const std::vector<Object *> &getObjects() const;
    void addObject(Object* object);
    void removeObject(Object* object);

    Type type;
    std::string room;
    Coordinate coordinate;
    Cell* up;
    Cell* down;
    Cell* left;
    Cell* right;

private:
    Cell(uint32_t x, uint32_t y);
    std::vector<Object*> objects;
};

} // namespace srgsim