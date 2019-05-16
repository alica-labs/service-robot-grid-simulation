#pragma once

#include "Cell.h"
#include <stdint.h>
#include <vector>

namespace grid_sim
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
    void setCell(uint32_t x, uint32_t y, Type type = Type::Floor);
    const std::vector<std::vector<Cell*>>& getGrid() { return this->grid; };
    uint32_t getSize();

private:
    std::vector<std::vector<Cell*>> grid = std::vector<std::vector<Cell*>>();

    /**
     * Lets the world grow on demand and
     * fills up non existing cells with Floor cells.
     * @param x
     * @param y
     */
    void growWorld(uint32_t x, uint32_t y);
};
} // namespace grid_sim