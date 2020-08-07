#include "srg/World.h"

#include "srg/world/Agent.h"
#include "srg/world/Cell.h"
#include "srg/world/Door.h"
#include "srg/world/Object.h"
#include "srg/world/Room.h"

#include <essentials/SystemConfig.h>
#include <essentials/IDManager.h>

#include <essentials/FileSystem.h>
#include <Tmx.h>

#include <iostream>

namespace srg
{
World::World(essentials::IDManager& idManager)
        : World(essentials::SystemConfig::getInstance().getConfigPath() + "textures/Department.tmx", idManager)
{
}

World::World(std::string tmxMapFile, essentials::IDManager& idManager)
        : sizeX(0)
        , sizeY(0)
{
    std::cout << "[World] Loading '" << tmxMapFile << "' world file!" << std::endl;
    Tmx::Map* map = new Tmx::Map();
    map->ParseFile(tmxMapFile);
    for (auto layer : map->GetTileLayers()) {
        // create room
        std::string roomName = layer->GetName();
        uint32_t roomIDInt = layer->GetProperties().GetIntProperty("ID");
        world::Room* room = this->addRoom(roomName, idManager.getID(roomIDInt));

        // create cells
        int roomType = 0;
        for (int x = 0; x < layer->GetWidth(); x++) {
            for (int y = 0; y < layer->GetHeight(); y++) {
                if (layer->GetTile(x, y).gid > 0) {
                    roomType = layer->GetTile(x, y).gid - 17;
                    std::shared_ptr<const world::Cell> cell = this->addCell(x, y, room);
                }
            }
        }

        if (room) {
            room->type = static_cast<world::RoomType>(roomType);
            std::cout << "[World] Added " << *room << std::endl;
        }
    }
}

World::~World()
{
    for (auto& room : rooms) {
        delete room.second;
    }
}

world::Room* World::addRoom(std::string name, essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Room* room = new world::Room(name, id);
    this->rooms.emplace(id, room);
    return room;
}

std::shared_ptr<world::Cell> World::addCell(uint32_t x, uint32_t y, world::Room* room)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    world::Coordinate cellCoordinate = world::Coordinate(x, y);

    if (this->cellGrid.find(cellCoordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(cellCoordinate);
    }

    std::shared_ptr<world::Cell> cell = std::shared_ptr<world::Cell>(new world::Cell(x, y));
    this->cellGrid.emplace(cell->coordinate, cell);

    // Left
    auto it = this->cellGrid.find(world::Coordinate(x - 1, y));
    if (it != this->cellGrid.end()) {
        cell->left = it->second;
        it->second->right = cell;
    }
    // Up
    it = this->cellGrid.find(world::Coordinate(x, y - 1));
    if (it != this->cellGrid.end()) {
        cell->up = it->second;
        it->second->down = cell;
    }
    // Right
    it = this->cellGrid.find(world::Coordinate(x + 1, y));
    if (it != this->cellGrid.end()) {
        cell->right = it->second;
        it->second->left = cell;
    }
    // Down
    it = this->cellGrid.find(world::Coordinate(x, y + 1));
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

std::shared_ptr<const world::Cell> World::getCell(const world::Coordinate& coordinate) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    if (this->cellGrid.find(coordinate) != this->cellGrid.end()) {
        return this->cellGrid.at(coordinate);
    }
    return nullptr;
}

std::vector<std::shared_ptr<const world::Object>> World::editObjects() {
    std::vector<std::shared_ptr<const world::Object>> objectList;
    for (auto& object : this->objects) {
        objectList.push_back(object.second);
    }
    return objectList;
}

bool World::placeObject(std::shared_ptr<world::Object> object, world::Coordinate coordinate)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto cellIter = this->cellGrid.find(coordinate);
    if (cellIter == this->cellGrid.end()) {
        return false;
    }

    if (!isPlacementAllowed(cellIter->second, object->getType())) {
        return false;
    }

    object->setParentContainer(cellIter->second);
    return true;
}

std::shared_ptr<const world::Object> World::getObject(world::ObjectType type) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    for (auto& object : this->objects) {
        if (object.second->getType() != type) {
            continue; // wrong type
        }

        return object.second;
    }
    return nullptr;
}

std::shared_ptr<const world::Object> World::getObject(essentials::IdentifierConstPtr id) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto objectEntry = this->objects.find(id);
    if (objectEntry == this->objects.end()) {
        return nullptr;
    }

    return objectEntry->second;
}

std::shared_ptr<world::Object> World::editObject(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto objectEntry = this->objects.find(id);
    if (objectEntry != this->objects.end()) {
        return objectEntry->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<const world::Agent> World::getAgent(essentials::IdentifierConstPtr id) const
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto agentEntry = this->agents.find(id);
    if (agentEntry != this->agents.end()) {
        return agentEntry->second;
    } else {
        return nullptr;
    }
}

std::shared_ptr<world::Agent> World::editAgent(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto agentEntry = this->agents.find(id);
    if (agentEntry != this->agents.end()) {
        return agentEntry->second;
    } else {
        return nullptr;
    }
}

const std::map<world::Coordinate, std::shared_ptr<world::Cell>>& World::getGrid()
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

std::shared_ptr<world::Agent> World::spawnAgent(essentials::IdentifierConstPtr id, world::ObjectType agentType)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    // create robot
    std::shared_ptr<world::Object> object = this->createOrUpdateObject(std::make_shared<srg::world::Object>(id, agentType));
    if (object->getParentContainer()) {
        // robot is already placed, maybe it was spawned already...
        return std::dynamic_pointer_cast<world::Agent>(object);
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    std::shared_ptr<const world::Cell> cell = nullptr;
    int x = 72;
    int y = 20;
    while (!cell || !isPlacementAllowed(cell, agentType)) {
        cell = this->getCell(world::Coordinate(x++, y));
//          cell = this->getCell(world::Coordinate(rand() % this->sizeX, rand() % this->sizeY));
    }

    // place robot
    if (this->placeObject(object, cell->coordinate)) {
        // only add robot into list, if it was placed correctly
        std::shared_ptr<world::Agent> robot = std::dynamic_pointer_cast<world::Agent>(object);
        this->addAgent(robot);
        return robot;
    } else {
        return nullptr;
    }
}

std::shared_ptr<world::Object> World::createOrUpdateObject(std::shared_ptr<world::Object> tmpObject)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::shared_ptr<world::Object> object = editObject(tmpObject->getID());
    if (!object) {
        switch (tmpObject->getType()) {
        case world::ObjectType::Robot:
        case world::ObjectType::Human:
            object = std::make_shared<world::Agent>(tmpObject->getID(), tmpObject->getType());
            break;
        case world::ObjectType::Door:
            object = std::make_shared<world::Door>(tmpObject->getID(), tmpObject->getState());
            break;
        default:
            object = std::make_shared<world::Object>(tmpObject->getID(), tmpObject->getType(), tmpObject->getState());
        }
        //        std::cout << "[World] Created " << *object;
        this->objects.emplace(object->getID(), object);
    }

    object->setType(tmpObject->getType());
    object->setState(tmpObject->getState());

    for (auto& childMsgObjectEntry : tmpObject->getObjects()) {
        object->addObject(createOrUpdateObject(childMsgObjectEntry.second));
    }

    return object;
}

void World::updateCell(world::Coordinate coordinate, std::vector<std::shared_ptr<world::Object>> objects, int64_t time)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto cellEntry = this->cellGrid.find(coordinate);
    if (cellEntry == this->cellGrid.end()) {
        return;
    }
    //    std::cout << "[World]" << *cellEntry->second << std::endl;
    cellEntry->second->timeOfLastUpdate = time;
    cellEntry->second->update(objects);
}

/**
 * Removes all objects that don't have coordinates, because
 * then they are unknown objects. For example, once recognized
 * but then vanished from the map (due to manipulation of another
 * robot).
 */
std::vector<std::shared_ptr<world::Object>> World::removeUnknownObjects()
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::vector<std::shared_ptr<world::Object>> unknownObjects;
    for (auto& objectEntry : this->objects) {
        if (objectEntry.second->getCoordinate().x < 0) {
            unknownObjects.push_back(objectEntry.second);
        }
    }
    for (auto object : unknownObjects) {
        this->objects.erase(object->getID());
    }
    return unknownObjects;
}

void World::removeObjectIfUnknown(essentials::IdentifierConstPtr objectID)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::shared_ptr<const world::Object> object = this->getObject(objectID);
    if (object && object->getCoordinate().x < 0) {
        this->objects.erase(object->getID());
    }
}

void World::moveObject(essentials::IdentifierConstPtr id, world::Direction direction)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::shared_ptr<world::Object> object = editObject(id);
    if (!object) {
        return;
    }
    std::shared_ptr<world::Cell> goalCell = getNeighbourCell(direction, object);
    if (!goalCell) {
        std::cerr << "[World] Cell does not exist! " << std::endl;
        return;
    }
    if (!isPlacementAllowed(goalCell, object->getType())) {
        std::cerr << "[World] Placement not allowed on " << goalCell->coordinate << " of type " << object->getType() << std::endl;
        return;
    }
    goalCell->addObject(object);
}

void World::displaceObject()
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);

    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::const_iterator objectIter;
    while (true) {
        objectIter = this->objects.begin();
        std::advance(objectIter, rand() % this->objects.size());
        if (objectIter->second->getType() == world::ObjectType::CupBlue || objectIter->second->getType() == world::ObjectType::CupYellow ||
                objectIter->second->getType() == world::ObjectType::CupRed) {
            if (objectIter->second->canBePickedUp(nullptr)) { // not sure, whether nullptr is ok
                world::Coordinate randomCoordinate = this->getRandomCoordinate();
                this->placeObject(objectIter->second, randomCoordinate);
                return;
            }
        }
    }
}

bool World::addAgent(std::shared_ptr<world::Agent> agent)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    auto agentEntry = this->agents.find(agent->getID());
    if (agentEntry == this->agents.end()) {
        this->agents.emplace(agent->getID(), agent);
        return true;
    } else {
        return false;
    }
}

void World::openDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::shared_ptr<world::Door> door = std::dynamic_pointer_cast<world::Door>(editObject(id));
    if (door) {
        door->setState(world::ObjectState::Open);
    } else {
        std::cout << "[World] No suitable door found with ID: " << *id << std::endl;
    }
}

void World::closeDoor(essentials::IdentifierConstPtr id)
{
    std::lock_guard<std::recursive_mutex> guard(dataMutex);
    std::shared_ptr<world::Door> door = std::dynamic_pointer_cast<world::Door>(editObject(id));
    if (door) {
        door->setState(world::ObjectState::Closed);
    } else {
        std::cout << "[World] No suitable door found with ID: " << *id << std::endl;
    }
}

const std::unordered_map<essentials::IdentifierConstPtr, world::Room*> World::getRooms() const
{
    return this->rooms;
}

const std::vector<world::Room*> World::getRooms(world::RoomType type) const
{
    std::vector<world::Room*> rooms;
    for (auto& room : this->rooms) {
        if (room.second->getType() == type) {
            rooms.push_back((world::Room* const) room.second);
        }
    }
    return rooms;
}

// INTERNAL METHODS

std::shared_ptr<world::Cell> World::getNeighbourCell(const world::Direction& direction, std::shared_ptr<world::Object> object)
{
    std::shared_ptr<const world::Cell> cell = std::dynamic_pointer_cast<const world::Cell>(object->getParentContainer());
    switch (direction) {
    case world::Direction::Left:
        return cell->left;
    case world::Direction::Up:
        return cell->up;
    case world::Direction::Right:
        return cell->right;
    case world::Direction::Down:
        return cell->down;
    default:
        std::cout << "[World] Unknown Direction: " << direction << std::endl;
        return nullptr;
    }
}

bool World::isPlacementAllowed(std::shared_ptr<const world::Cell> cell, world::ObjectType objectType) const
{
    if (cell->getType() == world::RoomType::Wall) {
        return false;
    }

    for (auto& objectEntry : cell->getObjects()) {
        if (objectEntry.second->getType() == world::ObjectType::Door) {
            return objectEntry.second->getState() == world::ObjectState::Open;
        }
    }

    return true;
}

srg::world::Coordinate World::getRandomCoordinate()
{
    // statistic evaluation of room
    int randRoomValue = rand() % 100;
    std::vector<srg::world::Room*> rooms;
    if (randRoomValue < 30) {
        // kitchen
        rooms = this->getRooms(srg::world::RoomType::Kitchen);
    } else if (randRoomValue < 80) {
        // office
        rooms = this->getRooms(srg::world::RoomType::Office);
    } else {
        // other room types
        for (auto& roomEntry : this->getRooms()) {
            switch (roomEntry.second->getType()) {
            case srg::world::RoomType::Kitchen:
            case srg::world::RoomType::Office:
            case srg::world::RoomType::Wall:
                continue;
            default:
                rooms.push_back(roomEntry.second);
            }
        }
    }

    // random coordinate in a random room
    randRoomValue = rand() % rooms.size();
    srg::world::Room* room = rooms[randRoomValue];

    auto& cells = room->getCells();
    std::map<srg::world::Coordinate, std::shared_ptr<srg::world::Cell>>::const_iterator cellIter;
    cellIter = cells.begin();
    std::advance(cellIter, rand() % cells.size());
    return cellIter->first;
}

std::recursive_mutex& World::getDataMutex()
{
    return this->dataMutex;
}

} // namespace srg
