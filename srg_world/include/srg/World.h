#pragma once

#include "srg/world/Coordinate.h"
#include "srg/world/Direction.h"
#include "srg/world/ObjectState.h"
#include "srg/world/ObjectType.h"
#include "srg/world/RoomType.h"

#include <srg/viz/Marker.h>

#include <essentials/IdentifierConstPtr.h>

#include <map>
#include <memory>
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
class Agent;
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

    // objects
    std::shared_ptr<const world::Object> getObject(world::ObjectType type) const;
    std::shared_ptr<const world::Object> getObject(essentials::IdentifierConstPtr id) const;
    std::shared_ptr<world::Object> editObject(essentials::IdentifierConstPtr id);
    void updateCell(world::Coordinate coordinate, std::vector<std::shared_ptr<world::Object>> objects);
    std::shared_ptr<world::Object> createOrUpdateObject(std::shared_ptr<world::Object> tmpObject);
    void removeUnknownObjects();
    void removeObjectIfUnknown(essentials::IdentifierConstPtr objectID);
    bool placeObject(std::shared_ptr<world::Object> object, world::Coordinate coordinate);
    void moveObject(essentials::IdentifierConstPtr id, world::Direction direction);

    // agents
    std::shared_ptr<world::Agent> spawnAgent(essentials::IdentifierConstPtr id, world::ObjectType agentType);
    std::shared_ptr<const world::Agent> getAgent(essentials::IdentifierConstPtr id) const;
    std::shared_ptr<world::Agent> editAgent(essentials::IdentifierConstPtr id);
    bool addAgent(std::shared_ptr<world::Agent> agent);

    // other
    void openDoor(essentials::IdentifierConstPtr id);
    void closeDoor(essentials::IdentifierConstPtr id);
    const std::unordered_map<essentials::IdentifierConstPtr, world::Room*> getRooms() const;
    const std::vector<world::Room*> getRooms(world::RoomType type) const;

private:
    bool isPlacementAllowed(const world::Cell* cell, world::ObjectType objectType) const;
    world::Cell* getNeighbourCell(const world::Direction& direction, std::shared_ptr<world::Object> object);
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
    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>> objects;
    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Agent>> agents;
    std::unordered_map<essentials::IdentifierConstPtr, world::Room*> rooms;
};
} // namespace srg