#include "srgsim/world/World.h"

#include "srgsim/world/Cell.h"
#include "srgsim/world/Door.h"
#include "srgsim/world/Object.h"
#include "srgsim/world/ServiceRobot.h"

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
            this->addCell(x, y)->type = static_cast<SpriteObjectType>(map->GetTileLayer(0)->GetTile(x, y).id);
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
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
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
    auto it = this->cellGrid.find(Coordinate(x, y - 1));
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
        auto it = this->cellGrid.find(Coordinate(x, y + 1));
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

const Cell* World::getCell(Coordinate coordinate) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    if (this->cellGrid.find(coordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(coordinate);
    }
    return nullptr;
}

bool World::placeObject(Object* object, Coordinate coordinate)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto cellIter = this->cellGrid.find(coordinate);
    if (cellIter == this->cellGrid.end()) {
        return false;
    }

    if (!isPlacementAllowed(cellIter->second, object->getType())) {
        return false;
    }

    object->setCell(cellIter->second);
    return true;
}

const Object* World::getObject(essentials::IdentifierConstPtr id)
{
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
}

ServiceRobot* World::getRobot(essentials::IdentifierConstPtr id)
{
    auto robotEntry = this->robots.find(id);
    if (robotEntry != this->robots.end()) {
        return robotEntry->second;
    } else {
        return nullptr;
    }
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

std::vector<SimPerceptions> World::createSimPerceptionsList()
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::vector<SimPerceptions> perceptions;
    for (auto& robotEntry : this->robots) {
        perceptions.push_back(robotEntry.second->createSimPerceptions(this));
    }
    return perceptions;
}

bool World::spawnRobot(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    // create robot
    Object* object = this->createOrUpdateObject(id, SpriteObjectType::Robot);
    if (object->getCell()) {
        // robot is already placed, maybe it was spawned already...
        return false;
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    const Cell* cell = nullptr;
    while (!cell || !isPlacementAllowed(cell, SpriteObjectType::Robot)) {
        cell = this->getCell(Coordinate(5, 5));
//        cell = this->getCell(Coordinate(rand() % this->sizeX, rand() % this->sizeY));
    }

    // place robot
    if (this->placeObject(object, cell->coordinate)) {
        // only add robot into list, if it was placed correctly
        this->addRobot(static_cast<ServiceRobot*>(object));
        return true;
    } else {
        return false;
    }
}

Object* World::createOrUpdateObject(essentials::IdentifierConstPtr id, SpriteObjectType type, ObjectState state)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    Object* object = editObject(id);
    if (!object) {
        switch (type) {
        case SpriteObjectType::Robot:
            object = new ServiceRobot(id);
            break;
        case SpriteObjectType::Door:
            object = new class Door(id, state);
            break;
        default:
            object = new Object(type, id, state);
        }
        std::cout << "World::createOrUpdateObject(): Created " << *object;
        this->objects.emplace(object->getID(), object);
        return object;
    }

    object->setType(type);
    object->setState(state);
    return object;
}

bool World::removeObject(srgsim::Object* object)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto entry = this->objects.find(object->getID());
    if (entry == this->objects.end()) {
        return false;
    }
    this->objects.erase(object->getID());
    object->deleteCell();
    return true;
}

void World::moveObject(essentials::IdentifierConstPtr id, Direction direction)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::cout << "World::moveObject(): ID: " << *id << " Direction: " << direction << std::endl;
    Object* object = editObject(id);
    if (!object) {
        return;
    }
    Cell* goalCell = getNeighbourCell(direction, object);
    if (!goalCell) {
        std::cerr << "World::moveObject(): Cell does not exist! " << std::endl;
        return;
    }
    if (!isPlacementAllowed(goalCell, object->getType())) {
        std::cerr << "World::moveObject(): Placement not allowed on " << goalCell->coordinate << " of type " << object->getType() << std::endl;
        return;
    }
    object->getCell()->removeObject(object);
    goalCell->addObject(object);
}

bool World::addRobot(srgsim::ServiceRobot* robot)
{
    auto robotEntry = this->robots.find(robot->getID());
    if (robotEntry == this->robots.end()) {
        this->robots.emplace(robot->getID(), robot);
        return true;
    } else {
        return false;
    }
}

void World::openDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    class Door* door = dynamic_cast<class Door*>(editObject(id));
    if (door) {
        door->setState(ObjectState::Open);
    } else {
        std::cout << "World::openDoor(): No suitable door found with ID: " << *id << std::endl;
    }
}

void World::closeDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    class Door* door = dynamic_cast<class Door*>(editObject(id));
    if (door) {
        door->setState(ObjectState::Closed);
    } else {
        std::cout << "World::closeDoor(): No suitable door found with ID: " << *id << std::endl;
    }
}

std::vector<Perception>& World::getMarkers()
{
    return this->markers;
}

void World::addMarker(Perception p)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    this->markers.push_back(p);
}

std::recursive_mutex& World::getDataMutex()
{
    return this->dataMutex;
}

std::vector<Object*> World::updateCell(CellPerceptions cellPerceptions) {
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::vector<Object*> objects;

    Coordinate coordinates = Coordinate(cellPerceptions.x, cellPerceptions.y);
    auto cellEntry = this->cellGrid.find(coordinates);
    if (cellEntry == this->cellGrid.end()) {
        std::cerr << "World::updateCell(): Coordinate " << coordinates << " does not exist in the world! " << std::endl;
        return objects;
    }

    // update objects itself
    for (srgsim::Perception perception : cellPerceptions.perceptions) {
        objects.push_back(this->createOrUpdateObject(perception.objectID, perception.type, perception.state));
    }

    // update association with cell
    cellEntry->second->update(objects);

    return objects;
}

// INTERNAL METHODS

Cell* World::getNeighbourCell(const Direction& direction, Object* object)
{
    switch (direction) {
        case Direction::Left:
        return object->getCell()->left;
        case Direction::Up:
        return object->getCell()->up;
        case Direction::Right:
        return object->getCell()->right;
        case Direction::Down:
        return object->getCell()->down;
    default:
        std::cout << "World: Unknown Direction: " << direction << "!" << std::endl;
        return nullptr;
    }
}

bool World::isPlacementAllowed(const Cell* cell, SpriteObjectType objectType) const
{
    if (cell->type == SpriteObjectType::Wall) {
        return false;
    }

    for (Object* object : cell->getObjects()) {
        if(object->getType() == SpriteObjectType::Door) {
            if (objectType == SpriteObjectType::Robot) {
                return object->getState() == ObjectState::Open;
            } else {
                return false;
            }
        }
    }

    return true;
}

Object* World::editObject(essentials::IdentifierConstPtr id)
{
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
}

} // namespace srgsim
