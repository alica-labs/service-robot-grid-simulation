#pragma once

#include "containers/Coordinate.h"

#include "SRGEnums.h"

#include <stdint.h>
#include <vector>
#include <map>

namespace srgsim
{
class Cell;
class Object;

/**
 * Coordinates are:
 * X (left to right)
 * Y (bottom to up)
 */
class World
{
public:
    World();
    ~World();

    Cell* addCell(uint32_t x, uint32_t y);
    Cell* getCell(Coordinate coordinate);

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<Coordinate, Cell*>& getGrid();

    bool placeObject(Object* object, Coordinate coordinate);

private:
    /**
     * Lets the world grow on demand and
     * fills up non existing cells with Floor cells.
     * @param x
     * @param y
     */
    void growWorld(uint32_t x, uint32_t y);

    std::map<Coordinate, Cell*> cellGrid;
    /**
     * Current field length
     */
    uint32_t sizeX;
    /**
     * Current field width
     */
    uint32_t sizeY;
};
} // namespace srgsim