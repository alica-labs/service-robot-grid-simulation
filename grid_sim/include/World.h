#pragma once

#include <vector>
#include "Cell.h"

namespace grid_sim {
    class Cell;

    /**
     * Coordinates are:
     * X (left to right)
     * Y (bottom to up)
     */
    class World {
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
        void setCell(u_int32_t x, u_int32_t y, Type type = Type::Floor);
        const std::vector<std::vector<Cell*>>& getGrid() { return this->grid; };
        u_int32_t getSize();
    private:
        std::vector<std::vector<Cell*>> grid = std::vector<std::vector<Cell*>>();

        /**
         * Lets the world grow on demand and
         * fills up non existing cells with Floor cells.
         * @param x
         * @param y
         */
        void growWorld(u_int32_t x, u_int32_t y);
    };
}