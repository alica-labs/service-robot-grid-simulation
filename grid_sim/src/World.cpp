#include "srgsim/World.h"

#include "srgsim/Cell.h"
#include "srgsim/Object.h"
#include "srgsim/Simulator.h"

#include <Tmx.h>

#include <iostream>

namespace srgsim
{
World::World()
{
    std::string textureFile = Simulator::getSelfPath() + "/textures/Department.tmx";
    std::cout << textureFile << " " << std::endl;
    Tmx::Map* map = new Tmx::Map();
    map->ParseFile(textureFile);
    for (int x = 0; x < map->GetTileLayer(0)->GetWidth(); x++) {
        for (int y = 0; y < map->GetTileLayer(0)->GetHeight(); y++) {
            this->addCell(x, y)->type = static_cast<Type>(map->GetTileLayer(0)->GetTile(x, y).gid);
        }
    }
}

World::~World()
{
    for (auto pair : this->cellGrid) {
        delete pair.second;
    }
}

Cell* World::addCell(uint32_t x, uint32_t y)
{
    if(this->cellGrid.size() == 0) {
        Cell* cell = new Cell(x, y);
        this->cellGrid.emplace(Coordinate(x, y), cell);
        this->sizeX = 1;
        this->sizeY = 1;
        return cell;
    }

    if (this->cellGrid.find(Coordinate(x, y)) == this->cellGrid.end()) {
        Cell* cell = new Cell(x, y);
        bool attached = false;
        // Left
        if (x > 0) {
            auto it = this->cellGrid.find(Coordinate(x - 1, y));
            if (it != this->cellGrid.end()) {
                cell->left = it->second;
                it->second->right = cell;
                attached = true;
            }
        }
        // Up
        auto it = this->cellGrid.find(Coordinate(x, y + 1));
        if (it != this->cellGrid.end()) {
            cell->up = it->second;
            it->second->down = cell;
            attached = true;
        }
        // Right
        if (x > 0) {
            auto it = this->cellGrid.find(Coordinate(x + 1, y));
            if (it != this->cellGrid.end()) {
                cell->right = it->second;
                it->second->left = cell;
                attached = true;
            }
        }
        // Down
        if (y > 0) {
            auto it = this->cellGrid.find(Coordinate(x, y - 1));
            if (it != this->cellGrid.end()) {
                cell->down = it->second;
                it->second->up = cell;
                attached = true;
            }
        }
        if (attached) {
            this->cellGrid.emplace(Coordinate(x, y), cell);
            if (x + 1 > this->sizeX) {
                this->sizeX = x + 1;
            }
            if (y + 1 > this->sizeY) {
                this->sizeY = y + 1;
            }
            return cell;
        } else {
            delete cell;
            return nullptr;
        }
    } else {
        return this->cellGrid.at(Coordinate(x, y));
    }
}

void World::growWorld(uint32_t x, uint32_t y)
{
    // grow for x dimension
    if (this->sizeX <= x) {
        for (uint32_t i = this->sizeX; i <= x; i++) {
            for (uint32_t j = 0; j < this->sizeY; j++) {
                this->cellGrid.emplace(Coordinate(i, j), new Cell(i, j));
            }
        }
        this->sizeX = x + 1;
    }

    // grow for y dimension
    if (this->sizeY <= y) {
        for (uint32_t i = 0; i < this->sizeX; i++) {
            for (uint32_t j = this->sizeY; j <= y; j++) {
                this->cellGrid.emplace(Coordinate(i, j), new Cell(i, j));
            }
        }
        this->sizeY = y + 1;
    }
}

Cell* World::getCell(Coordinate coordinate)
{
    if (this->cellGrid.find(coordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(coordinate);
    }
    return nullptr;
}

bool World::placeObject(Object* object, Coordinate coordinate) {
    auto cellIter = this->cellGrid.find(coordinate);
    if (cellIter == this->cellGrid.end()) {
        return false;
    }

    cellIter->second->objects.push_back(object);
    object->setCell(cellIter->second);

    return true;
}

const std::map<Coordinate, Cell*>& World::getGrid()
{
    return this->cellGrid;
}

uint32_t World::getSizeX() const
{
    return sizeX;
}

uint32_t World::getSizeY() const
{
    return sizeY;
}
} // namespace srgsim
