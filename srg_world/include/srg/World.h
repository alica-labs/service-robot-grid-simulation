#pragma once

#include "srg/world/Coordinate.h"
#include "srg/world/ObjectType.h"
#include "srg/world/Direction.h"
#include "srg/world/ObjectState.h"

#include <srg/viz/Marker.h>

#include <essentials/IdentifierConstPtr.h>

#include <map>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace essentials
{
class IDManager;
} // namespace essentials
namespace srg
{
namespace world
{
class Cell;
class Object;
class ServiceRobot;
class Room;
} // namespace world

/**
 * Coordinates are:
 * X (left to right)
 * Y (top to down)
 */
class World
{
public:
    World(essentials::IDManager* idManager);
    World(std::string tmxMapFile, essentials::IDManager* idManager);
    ~World();

    world::Cell* addCell(uint32_t x, uint32_t y, world::Room* room);
    const world::Cell* getCell(const world::Coordinate& coordinate) const;

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<world::Coordinate, world::Cell*>& getGrid();
    std::recursive_mutex& getDataMutex();

//    std::vector<world::Object*> updateCell(srgsim::CellPerceptions cellPerceptions);

    // objects
    const world::Object* getObject(world::ObjectType type) const;
    const world::Object* getObject(essentials::IdentifierConstPtr id) const;
    world::Object* editObject(essentials::IdentifierConstPtr id);
    void updateCell(world::Coordinate coordinate, std::vector<world::Object*> objects);
    world::Object* createOrUpdateObject(essentials::IdentifierConstPtr id, world::ObjectType type, world::ObjectState state = world::ObjectState::Undefined,
            essentials::IdentifierConstPtr robotID = nullptr);
    bool removeObject(world::Object* object);
    bool placeObject(world::Object* object, world::Coordinate coordinate);
    void moveObject(essentials::IdentifierConstPtr id, world::Direction direction);

    // robots
    world::ServiceRobot* spawnRobot(essentials::IdentifierConstPtr id);
    const world::ServiceRobot* getRobot(essentials::IdentifierConstPtr id) const;
    world::ServiceRobot* editRobot(essentials::IdentifierConstPtr id);
    bool addRobot(world::ServiceRobot* robot);

    // other
    void openDoor(essentials::IdentifierConstPtr id);
    void closeDoor(essentials::IdentifierConstPtr id);

private:
    bool isPlacementAllowed(const world::Cell* cell, world::ObjectType objectType) const;
    world::Cell* getNeighbourCell(const world::Direction& direction, world::Object* object);
    world::Room* addRoom(std::string name, essentials::IdentifierConstPtr id);

    std::map<world::Coordinate, world::Cell*> cellGrid;
    /**
     * Current field length
     */
    uint32_t sizeX;
    /**
     * Current field width
     */
    uint32_t sizeY;

    mutable std::recursive_mutex dataMutex;
    std::unordered_map<essentials::IdentifierConstPtr, world::Object*> objects;
    std::unordered_map<essentials::IdentifierConstPtr, world::ServiceRobot*> robots;
    std::unordered_map<essentials::IdentifierConstPtr, world::Room*> rooms;
};
} // namespace srg