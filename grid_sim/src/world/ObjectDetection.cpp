#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/TaskType.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/CellPerceptions.h"

#include <SystemConfig.h>
#include <cnc_geometry/Calculator.h>

#include <math.h>

namespace srgsim
{
ObjectDetection::ObjectDetection(srgsim::ServiceRobot* robot)
        : Sensor(robot)
{
    this->sc = essentials::SystemConfig::getInstance();
    this->sightLimit = (*sc)["ObjectDetection"]->get<uint32_t>("sightLimit", NULL);
}

std::vector<CellPerceptions> ObjectDetection::createPerceptions(World* world)
{
    // collect cells in vision
    Coordinate ownCoord = this->robot->getCell()->coordinate;
    std::map<Coordinate, const Cell*> cellsInVision;
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += M_PI / 90) { // PI/90 <=> 2 degree resolution
        int32_t xDelta = round(sin(currentDegree) * sightLimit);
        int32_t yDelta = round(cos(currentDegree) * sightLimit);
        Perception p;
        p.type = SpriteObjectType::CupRed;
        p.x = ownCoord.x + xDelta;
        p.y = ownCoord.y + yDelta;
        world->addMarker(p);

        std::vector<const Cell*> currentCells = this->collectCells(ownCoord, Coordinate(ownCoord.x + xDelta, ownCoord.y + yDelta), world);
        for (const Cell* cell : currentCells) {
            // add only cells that are not already collected
            if (cellsInVision.find(cell->coordinate) == cellsInVision.end()) {
                cellsInVision.emplace(cell->coordinate, cell);
            }
        }
    }

    // collect objects as perceptions
    std::vector<CellPerceptions> cellPerceptionsList;
    for (auto& entry : cellsInVision) {

        // for debug purpose
        Perception p;
        p.type = SpriteObjectType::Unknown;
        p.x = entry.second->coordinate.x;
        p.y = entry.second->coordinate.y;
        world->addMarker(p);

        const std::vector<Object*>& objects = entry.second->getObjects();
        CellPerceptions cellPerceptions;
        cellPerceptions.x = entry.second->coordinate.x;
        cellPerceptions.y = entry.second->coordinate.y;
        for (Object* object : objects) {
            Perception p;
            p.objectID = object->getID();
            p.type = object->getType();
            p.state = object->getState();
            p.x = cellPerceptions.x;
            p.y = cellPerceptions.y;
            cellPerceptions.perceptions.push_back(p);
        }
        cellPerceptionsList.push_back(cellPerceptions);
    }

    return cellPerceptionsList;
}

std::vector<const Cell*> ObjectDetection::collectCells(Coordinate start, Coordinate end, World* world)
{
    std::vector<const Cell*> cells;
    cells.push_back(world->getCell(start));

    int32_t sign_x = ((int32_t)end.x - (int32_t)start.x) > 0 ? 1 : -1;
    int32_t sign_y = ((int32_t)end.y - (int32_t)start.y) > 0 ? 1 : -1;

    Coordinate currentPoint(start.x, start.y);
    Coordinate pointStepX(start.x, start.y);
    Coordinate pointStepY(start.x, start.y);
    while(true) {
        if (currentPoint == end){
            break;
        }
        pointStepX = Coordinate(currentPoint.x+sign_x,currentPoint.y);
        pointStepY = Coordinate(currentPoint.x,currentPoint.y+sign_y);
        double distanceStepX = geometry::distancePointToLineSegmentCalc(pointStepX.x, pointStepX.y, start.x, start.y, end.x, end.y);
        double distanceStepY = geometry::distancePointToLineSegmentCalc(pointStepY.x, pointStepY.y, start.x, start.y, end.x, end.y);

//        std::cout << "CurrentPoint: " << currentPoint << " StepX: " << pointStepX << " StepY: " << pointStepY << " DistX: " << distanceStepX << " DistY: " << distanceStepY << std::endl;
        if (distanceStepX < distanceStepY) {
            currentPoint = pointStepX;
        } else {
            currentPoint = pointStepY;
        }

        const Cell* cell = world->getCell(currentPoint);
        if (!cell || cell->type != SpriteObjectType::Floor) {
            break;
        }
        cells.push_back(cell);
    }
    return cells;
}
} // namespace srgsim