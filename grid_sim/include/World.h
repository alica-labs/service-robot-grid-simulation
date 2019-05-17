#pragma once

#include "container/Coordinate.h"

#include "SRGEnums.h"

#include <stdint.h>
#include <vector>
#include <map>

namespace srgsim
{
class Cell;

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

    /**
     * Calls growWorld if necessary!
     * Sets the type of the cell at (x,y)
     * @param x
     * @param y
     * @param type
     */
    Cell* addCell(uint32_t x, uint32_t y);
    const std::map<Coordinate, Cell*>& getGrid();

    Cell* getCell(Coordinate coordinate);

private:
    //std::vector<std::vector<Cell*>> grid = std::vector<std::vector<Cell*>>();

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
public:
    uint32_t getSizeX() const;

    uint32_t getSizeY() const;

private:
    /**
     * Current field width
     */
    uint32_t sizeY;
};
} // namespace srgsim