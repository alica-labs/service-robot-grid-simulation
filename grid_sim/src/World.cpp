#include "World.h"

#include <iostream>

namespace grid_sim
{
World::World()
{
    // always init world with a single floor cell
    this->grid.push_back(std::vector<Cell*>({new Cell(0, 0)}));
    this->grid[0][0]->type = Type::Floor;
}

World::~World()
{
    for (std::vector<Cell*>& column : this->grid) {
        for (Cell* cell : column) {
            delete cell;
        }
    }
}

void World::setCell(uint32_t x, uint32_t y, Type type)
{
    this->growWorld(x, y);
    this->grid[x][y]->type = type;
}

void World::growWorld(uint32_t x, uint32_t y)
{
    // grow for x dimension
    if (this->grid.size() <= x) {
        for (uint32_t i = this->grid.size(); i <= x; i++) {
            this->grid.emplace_back();
            for (uint32_t j = 0; j < this->grid[0].size(); j++) {
                this->grid[i].push_back(new Cell(i, j));
            }
        }
    }

    // grow for y dimension
    if (this->grid[0].size() <= y) {
        for (uint32_t i = 0; i < this->grid.size(); i++) {
            for (uint32_t j = this->grid[i].size(); j <= y; j++) {
                this->grid[i].push_back(new Cell(i, j));
            }
        }
    }
}

uint32_t World::getSize()
{
    return this->grid.size();
}
} // namespace grid_sim
