#pragma once

#include "srgsim/containers/Coordinate.h"

#include "srgsim/SRGEnums.h"
#include "srgsim/containers/SimPerceptions.h"

#include <essentials/IdentifierConstPtr.h>

#include <map>
#include <mutex>
#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace essentials {
    class SystemConfig;
}
namespace srgsim
{
class Cell;
class Object;
class ServiceRobot;

/**
 * Coordinates are:
 * X (left to right)
 * Y (bottom to up)
 */
class World
{
public:
    World();
    World(std::string tmxMapFile, bool robot = false);
    ~World();

    Cell* addCell(uint32_t x, uint32_t y);
    const Cell* getCell(Coordinate coordinate) const;

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<Coordinate, Cell*>& getGrid();

    bool placeObject(Object* object, Coordinate coordinate);
    const Object* getObject(essentials::IdentifierConstPtr id);
    std::vector<SimPerceptions> createSimPerceptions();
    bool spawnRobot(essentials::IdentifierConstPtr id);
    Object* addObject(essentials::IdentifierConstPtr id, Type type);
    bool addRobot(ServiceRobot* robot);
    void moveObject(essentials::IdentifierConstPtr id, Direction direction);
    void openDoor(essentials::IdentifierConstPtr id);
    void closeDoor(essentials::IdentifierConstPtr id);
    bool pickupObject(essentials::IdentifierConstPtr id);

private:
    bool isPlacementAllowed(const Cell* cell, Type objectType) const;
    Cell* getNeighbourCell(const Direction& direction, Object* object);
    Object* editObject(essentials::IdentifierConstPtr id);

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
};
} // namespace srgsim