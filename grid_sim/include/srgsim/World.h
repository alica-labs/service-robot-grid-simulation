#pragma once

#include "containers/Coordinate.h"

#include "srgsim/SRGEnums.h"
#include "srgsim/containers/SimPerceptions.h"

#include <essentials/IdentifierConstPtr.h>

#include <stdint.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <mutex>

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
    World(std::string tmxMapFile);
    ~World();

    Cell* addCell(uint32_t x, uint32_t y);
    Cell* getCell(Coordinate coordinate);

    uint32_t getSizeX() const;
    uint32_t getSizeY() const;
    const std::map<Coordinate, Cell*>& getGrid();

    bool placeObject(Object* object, Coordinate coordinate);
    std::vector<SimPerceptions> createSimPerceptions();
    bool spawnRobot(essentials::IdentifierConstPtr id);
    Object* addObject(essentials::IdentifierConstPtr id, Type type);
    void moveObject(const essentials::Identifier* id, Direction direction);

private:
    bool isPlacementAllowed(Cell* cell, Type objectType);
    Cell* getNeighbourCell(const Direction& direction, Object* object);

    std::map<Coordinate, Cell*> cellGrid;
    /**
     * Current field length
     */
    uint32_t sizeX;
    /**
     * Current field width
     */
    uint32_t sizeY;

    std::mutex dataMutex;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> objects;
    std::unordered_map<essentials::IdentifierConstPtr, ServiceRobot*> robots;
};
} // namespace srgsim