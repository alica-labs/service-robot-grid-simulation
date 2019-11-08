#pragma once

#include "srgsim/containers/Coordinate.h"
#include "srgsim/containers/SimCommand.h"
#include "srgsim/containers/SimPerceptions.h"
#include "srgsim/world/Direction.h"
#include "ObjectType.h"

#include <essentials/IdentifierConstPtr.h>

#include <map>
#include <mutex>

#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace essentials
{
class SystemConfig;
class IDManager;
}
namespace srgsim
{
class Cell;
class Object;
class ServiceRobot;
class Room;

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

    Cell* addCell(uint32_t x, uint32_t y, Room* room);
    const Cell* getCell(const Coordinate& coordinate) const;

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<Coordinate, Cell*>& getGrid();
    std::vector<Perception>& getMarkers();
    void addMarker(Perception p);
    std::recursive_mutex& getDataMutex();

    std::vector<SimPerceptions> createSimPerceptionsList();

    std::vector<Object*> updateCell(srgsim::CellPerceptions cellPerceptions);

    // objects
    const Object* getObject(essentials::IdentifierConstPtr id) const;
    Object* editObject(essentials::IdentifierConstPtr id);
    Object* createOrUpdateObject(essentials::IdentifierConstPtr id, ObjectType type, ObjectState state = ObjectState::Undefined,
            essentials::IdentifierConstPtr robotID = nullptr);
    bool removeObject(Object* object);
    bool placeObject(Object* object, Coordinate coordinate);
    void moveObject(essentials::IdentifierConstPtr id, Direction direction);

    // robots
    bool spawnRobot(essentials::IdentifierConstPtr id);
    const ServiceRobot* getRobot(essentials::IdentifierConstPtr id) const;
    ServiceRobot* editRobot(essentials::IdentifierConstPtr id);
    bool addRobot(ServiceRobot* robot);

    // other
    void openDoor(essentials::IdentifierConstPtr id);
    void closeDoor(essentials::IdentifierConstPtr id);

private:
    bool isPlacementAllowed(const Cell* cell, ObjectType objectType) const;
    Cell* getNeighbourCell(const Direction& direction, Object* object);
    Room* addRoom(std::string name, essentials::IdentifierConstPtr id);

    std::map<Coordinate, Cell*> cellGrid;
    /**
     * Current field length
     */
    uint32_t sizeX;
    /**
     * Current field width
     */
    uint32_t sizeY;

    mutable std::recursive_mutex dataMutex;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> objects;
    std::unordered_map<essentials::IdentifierConstPtr, ServiceRobot*> robots;
    std::unordered_map<essentials::IdentifierConstPtr, Room*> rooms;
    std::vector<Perception> markers;
};
} // namespace srgsim