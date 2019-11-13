#include "srgsim/world/World.h"

#include "srgsim/world/Cell.h"
#include "srgsim/world/Door.h"
#include "srgsim/world/Manipulation.h"
#include "srgsim/world/Object.h"
#include "srgsim/world/Room.h"
#include "srgsim/world/ServiceRobot.h"

#include <SystemConfig.h>
#include <essentials/IDManager.h>

#include <FileSystem.h>
#include <Tmx.h>

#include <iostream>

namespace srgsim
{
World::World(essentials::IDManager* idManager)
        : World(essentials::SystemConfig::getInstance()->getConfigPath() + "/textures/Department.tmx", idManager)
{
}

World::World(std::string tmxMapFile, essentials::IDManager* idManager) : sizeX(0), sizeY(0)
{
    std::cout << "[World] Loading '" << tmxMapFile << "' world file!" << std::endl;
    Tmx::Map* map = new Tmx::Map();
    map->ParseFile(tmxMapFile);
    for (auto layer : map->GetTileLayers()) {
        // create room
        std::string roomName = layer->GetName();
        uint32_t roomIDInt = layer->GetProperties().GetIntProperty("ID");
        Room* room = this->addRoom(roomName, idManager->getID(roomIDInt));

        // create cells
        int roomType = 0;
        for (int x = 0; x < layer->GetWidth(); x++) {
            for (int y = 0; y < layer->GetHeight(); y++) {
                if (layer->GetTile(x, y).gid > 0) {
                    roomType = layer->GetTile(x, y).gid - 17;
                    Cell* cell = this->addCell(x, y, room);
                }
            }
        }

        if (room) {
            room->type = static_cast<RoomType>(roomType);
            std::cout << "[World] Added " << *room << std::endl;
        }
    }
}

World::~World()
{
    for (auto pair : cellGrid) {
        delete pair.second;
    }
    for (auto& object : objects) {
        delete object.second;
    }
    for (auto& room : rooms) {
        delete room.second;
    }
}

Room* World::addRoom(std::string name, essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    Room* room = new Room(name, id);
    this->rooms.emplace(id, room);
    return room;
}

Cell* World::addCell(uint32_t x, uint32_t y, Room* room)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    Coordinate cellCoordinate = Coordinate(x, y);

    if (this->cellGrid.find(cellCoordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(cellCoordinate);
    }

    Cell* cell = new Cell(x, y);
    this->cellGrid.emplace(cell->coordinate, cell);

    // Left
    auto it = this->cellGrid.find(Coordinate(x - 1, y));
    if (it != this->cellGrid.end()) {
        cell->left = it->second;
        it->second->right = cell;
    }
    // Up
    it = this->cellGrid.find(Coordinate(x, y - 1));
    if (it != this->cellGrid.end()) {
        cell->up = it->second;
        it->second->down = cell;
    }
    // Right
    it = this->cellGrid.find(Coordinate(x + 1, y));
    if (it != this->cellGrid.end()) {
        cell->right = it->second;
        it->second->left = cell;
    }
    // Down
    it = this->cellGrid.find(Coordinate(x, y + 1));
    if (it != this->cellGrid.end()) {
        cell->down = it->second;
        it->second->up = cell;
    }

    if (x + 1 > this->sizeX) {
        this->sizeX = x + 1;
    }
    if (y + 1 > this->sizeY) {
        this->sizeY = y + 1;
    }

    room->addCell(cell);

    return cell;
}

const Cell* World::getCell(const Coordinate& coordinate) const
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

const Object* World::getObject(essentials::IdentifierConstPtr id) const
{
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
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

const ServiceRobot* World::getRobot(essentials::IdentifierConstPtr id) const
{
    auto robotEntry = this->robots.find(id);
    if (robotEntry != this->robots.end()) {
        return robotEntry->second;
    } else {
        return nullptr;
    }
}

ServiceRobot* World::editRobot(essentials::IdentifierConstPtr id)
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
    Object* object = this->createOrUpdateObject(id, ObjectType::Robot);
    if (object->getCell()) {
        // robot is already placed, maybe it was spawned already...
        return false;
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    const Cell* cell = nullptr;
    while (!cell || !isPlacementAllowed(cell, ObjectType::Robot)) {
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

Object* World::createOrUpdateObject(essentials::IdentifierConstPtr id, ObjectType type, ObjectState state, essentials::IdentifierConstPtr robotID)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    Object* object = editObject(id);
    if (!object) {
        switch (type) {
        case ObjectType::Robot:
            object = new ServiceRobot(id);
            break;
        case ObjectType::Door:
            object = new class Door(id, state);
            break;
        default:
            object = new Object(type, id, state);
        }
        std::cout << "[World] Created " << *object;
        this->objects.emplace(object->getID(), object);
    }

    object->setType(type);
    object->setState(state);
    // dirty hack, I know! :P
    // TODO: Adapt messages from simulator and allow to set object accordingly
    if (state == ObjectState::Carried) {
        if (ServiceRobot* robot = this->editRobot(robotID)) {
            //            std::cout << "World::createOrUpdateObject(): Robot " << robotID << " carries " << type << std::endl;
            robot->manipulation->carriedObject = object;
        } else {
            //            std::cout << "World::createOrUpdateObject(): Robot unknown! " << robotID << std::endl;
        }
    }
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
    object->editCell()->removeObject(object);
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

std::vector<Object*> World::updateCell(CellPerceptions cellPerceptions)
{
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
        //        std::cout << "World::updateCell(): " << perception << std::endl;
        objects.push_back(this->createOrUpdateObject(perception.objectID, perception.type, perception.state, perception.robotID));
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
        std::cout << "[World] Unknown Direction: " << direction << std::endl;
        return nullptr;
    }
}

bool World::isPlacementAllowed(const Cell* cell, ObjectType objectType) const
{
    if (cell->getType() == RoomType::Wall) {
        return false;
    }

    for (Object* object : cell->getObjects()) {
        if (object->getType() == ObjectType::Door) {
            if (objectType == ObjectType::Robot) {
                return object->getState() == ObjectState::Open;
            } else {
                return false;
            }
        }
    }

    return true;
}

} // namespace srgsim
