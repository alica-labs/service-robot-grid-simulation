#include "srgsim/World.h"

#include "srgsim/Cell.h"
#include "srgsim/Object.h"
#include "srgsim/ServiceRobot.h"

#include <FileSystem.h>
#include <Tmx.h>

#include <iostream>

namespace srgsim
{
World::World()
        : World(essentials::FileSystem::getSelfPath() + "/textures/Department.tmx")
{
}

World::World(std::string tmxMapFile)
{
    std::cout << "srgsim::World(): Loading '" << tmxMapFile << "' world file!" << std::endl;
    Tmx::Map* map = new Tmx::Map();
    map->ParseFile(tmxMapFile);
    for (int x = 0; x < map->GetTileLayer(0)->GetWidth(); x++) {
        for (int y = 0; y < map->GetTileLayer(0)->GetHeight(); y++) {
            this->addCell(x, y)->type = static_cast<Type>(map->GetTileLayer(0)->GetTile(x, y).id);
        }
    }
}

World::~World()
{
    for (auto pair : this->cellGrid) {
        delete pair.second;
    }
    for (auto& object : objects) {
        delete object.second;
    }
}

Cell* World::addCell(uint32_t x, uint32_t y)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    if (this->cellGrid.size() == 0) {
        Cell* cell = new Cell(x, y);
        this->cellGrid.emplace(Coordinate(x, y), cell);
        this->sizeX = 1;
        this->sizeY = 1;
        return cell;
    }

    if (this->cellGrid.find(Coordinate(x, y)) != this->cellGrid.end()) {
        return this->cellGrid.at(Coordinate(x, y));
    }

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
}

Cell* World::getCell(Coordinate coordinate)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    if (this->cellGrid.find(coordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(coordinate);
    }
    return nullptr;
}

bool World::placeObject(Object* object, Coordinate coordinate)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    auto cellIter = this->cellGrid.find(coordinate);
    if (cellIter == this->cellGrid.end()) {
        return false;
    }

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

std::vector<SimPerceptions> World::createSimPerceptions()
{
    std::lock_guard<std::mutex> guard(dataMutex);
    std::vector<SimPerceptions> perceptions;
    for (auto& robotEntry : this->robots) {
        perceptions.push_back(robotEntry.second->createSimPerceptions(this));
    }
    return perceptions;
}

bool World::spawnRobot(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    // create robot
    Object* object = this->addObject(id, Type::Robot);
    if (object->getCell()) {
        // robot is already placed, maybe it was spawned already...
        return false;
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    Cell* cell = nullptr;
    while (!cell || !isPlacementAllowed(cell, Type::Robot)) {
        cell = this->getCell(Coordinate(rand() % this->sizeX, rand() % this->sizeY));
    }

    // place robot
    if (this->placeObject(object, cell->coordinate)) {
        // only add robot into list, if it was placed correctly
        this->robots.emplace(object->getID(), static_cast<ServiceRobot*>(object));
        return true;
    } else {
        return false;
    }
}

Object* World::addObject(essentials::IdentifierConstPtr id, Type type)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    auto objectEntry = this->objects.find(essentials::IdentifierConstPtr(id));
    if (objectEntry == this->objects.end()) {
        Object* object;
        if (type == Type::Robot) {
            object = new ServiceRobot(id);
        } else {
            object = new Object(type, id);
        }
        this->objects.emplace(object->getID(), object);
        return object;
    } else {
        if (objectEntry->second->getType() != type) {
            std::cerr << "World::addObject(): Object ID " << id << " is already known as type (" << objectEntry->second->getType()
                      << "), but requested type is (" << type << ")!" << std::endl;
            return nullptr;
        } else {
            return objectEntry->second;
        }
    }
}

void World::moveObject(const essentials::Identifier* id, Direction direction)
{
    std::lock_guard<std::mutex> guard(dataMutex);
    auto iter = objects.find(essentials::IdentifierConstPtr(id));
    if (iter == objects.end()) {
        std::cout << "World::moveObject: unknown object ID " << *id << " requested!" << std::endl;
        return;
    }
    Cell* goalCell = getNeighbourCell(direction, iter->second);
    if (!goalCell) {
        return;
    }
    if (!isPlacementAllowed(goalCell, iter->second->getType())) {
        return;
    }
    iter->second->getCell()->removeObject(iter->second);
    goalCell->addObject(iter->second);
}

// INTERNAL METHODS

Cell* World::getNeighbourCell(const Direction& direction, Object* object)
{
    switch (direction) {
    case Left:
        return object->getCell()->left;
    case Up:
        return object->getCell()->up;
    case Right:
        return object->getCell()->right;
    case Down:
        return object->getCell()->down;
    default:
        std::cout << "World: Unknown Direction: " << direction << "!" << std::endl;
        return nullptr;
    }
}

bool World::isPlacementAllowed(Cell* cell, Type objectType)
{
    switch (objectType) {
    case Type::Robot:
        return cell->type == Type::Floor;
    default:
        return !(cell->type == Type::Door || cell->type == Type::Wall);
    }
}
} // namespace srgsim
