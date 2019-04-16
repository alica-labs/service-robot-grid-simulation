#include "World.h"

#include <iostream>

namespace grid_sim{
    World::World() {
        // always init world with a single floor cell
        this->grid.push_back(std::vector<Cell*>({new Cell()}));
        this->grid[0][0]->type = Type::Floor;
    }

    World::~World() {
        for (std::vector<Cell*> column : this->grid) {
            for (Cell* cell : column) {
                delete cell;
            }
        }
    }

    void World::setCell(u_int32_t x, u_int32_t y, Type type) {
        this->growWorld(x,y);
        this->grid[x][y]->type = type;
    }

    void World::growWorld(u_int32_t x, u_int32_t y) {
        // grow for x dimension
        if (this->grid.size() <= x) {
            for (u_int32_t i = this->grid.size(); i <= x; i++) {
                this->grid.push_back(std::vector<Cell*>());
                for (u_int32_t j = 0; j < this->grid[0].size(); j++) {
                    this->grid[i].push_back(new Cell());
                }
            }
        }

        // grow for y dimension
        if (this->grid[0].size() <= y) {
            for (u_int32_t i = 0; i < this->grid.size(); i++) {
                for (u_int32_t j = this->grid[i].size(); j <= y; j++) {
                    this->grid[i].push_back(new Cell());
                }
            }
        }
    }
}

