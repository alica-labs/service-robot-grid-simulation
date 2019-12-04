#include "srg/sim/Sensor.h"

#include "srg/Simulator.h"
#include "srg/sim/SimulatedAgent.h"
#include "srg/sim/containers/CellPerception.h"

#include <srg/World.h>
#include <srg/world/Cell.h>
#include <srg/world/Door.h>
#include <srg/world/Coordinate.h>

#include <SystemConfig.h>
#include <cnc_geometry/Calculator.h>

namespace srg
{
namespace sim
{
Sensor::Sensor(srg::sim::SimulatedAgent* robot)
        : robot(robot)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
}

std::vector<containers::CellPerception> Sensor::createPerceptions(srg::Simulator* simulator)
{
    // collect cells in vision
    world::Coordinate from = this->robot->getCoordinate();
    std::map<world::Coordinate, const world::Cell*> cellsInVision;
    double increment = atan2(1, sightLimit + 1);
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += increment) { // PI/90 <=> 2 degree resolution
        int32_t xDelta = round(sin(currentDegree) * sightLimit);
        int32_t yDelta = round(cos(currentDegree) * sightLimit);
        world::Coordinate to = world::Coordinate(from.x + xDelta, from.y + yDelta);
        // for debug purpose
//        viz::Marker marker(to);
//        marker.type = viz::SpriteType::Default;
//        simulator->addMarker(marker);

        std::vector<const world::Cell*> currentCells = this->collectCells(from, to, simulator->getWorld());
        for (const world::Cell* cell : currentCells) {
            // add only cells that are not already collected
            if (cellsInVision.find(cell->coordinate) == cellsInVision.end()) {
                cellsInVision.emplace(cell->coordinate, cell);
            }
        }
    }

    // collect objects as perceptions
    std::vector<containers::CellPerception> cellPerceptionsList;
    for (auto& entry : cellsInVision) {
        // for debug purpose
//        viz::Marker marker(entry.second->coordinate);
//        marker.type = viz::SpriteType::Unknown;
//        simulator->addMarker(marker);

        auto& objects = entry.second->getObjects();
        containers::CellPerception cellPerceptions;
        cellPerceptions.x = entry.second->coordinate.x;
        cellPerceptions.y = entry.second->coordinate.y;
        for (auto& objectEntry : objects) {
            cellPerceptions.objects.push_back(objectEntry.second);
        }
        cellPerceptionsList.push_back(cellPerceptions);
    }

    return cellPerceptionsList;
}

std::vector<const world::Cell*> Sensor::collectCells(world::Coordinate start, world::Coordinate end, srg::World* world)
{
    std::vector<const world::Cell*> cells;
    cells.push_back(world->getCell(start));

    int32_t sign_x = ((int32_t) end.x - (int32_t) start.x) > 0 ? 1 : -1;
    int32_t sign_y = ((int32_t) end.y - (int32_t) start.y) > 0 ? 1 : -1;

    world::Coordinate currentPoint(start.x, start.y);
    world::Coordinate pointStepX(start.x, start.y);
    world::Coordinate pointStepY(start.x, start.y);
    while (currentPoint != end) {
        // set next point
        pointStepX = world::Coordinate(currentPoint.x + sign_x, currentPoint.y);
        pointStepY = world::Coordinate(currentPoint.x, currentPoint.y + sign_y);
        double distanceStepX = geometry::distancePointToLineSegmentCalc(pointStepX.x, pointStepX.y, start.x, start.y, end.x, end.y);
        double distanceStepY = geometry::distancePointToLineSegmentCalc(pointStepY.x, pointStepY.y, start.x, start.y, end.x, end.y);
        if (distanceStepX < distanceStepY) {
            currentPoint = pointStepX;
        } else {
            currentPoint = pointStepY;
        }

        // check if sight is blocked in this cell
        const world::Cell* cell = world->getCell(currentPoint);
        if (!cell || cell->getType() == world::RoomType::Wall) {
            break;
        }
        bool sightBlocked = false;
        for (auto& objectEntry : cell->getObjects()) {
            if (const srg::world::Door* door = dynamic_cast<const srg::world::Door*>(objectEntry.second)) {
                sightBlocked = !door->isOpen();
            }
        }
        if (sightBlocked) {
            break;
        }
        cells.push_back(cell);
    }
    return cells;
}
} // namespace sim
} // namespace srg