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
    Coordinate from = this->robot->getCell()->coordinate;
    std::map<Coordinate, const Cell*> cellsInVision;
    double increment = atan2(1,sightLimit+1);
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += increment) { // PI/90 <=> 2 degree resolution
        int32_t xDelta = round(sin(currentDegree) * sightLimit);
        int32_t yDelta = round(cos(currentDegree) * sightLimit);
        Coordinate to = Coordinate(from.x + xDelta, from.y + yDelta);
        // for debug purpose
        Perception p;
        p.type = ObjectType::CupRed;
        p.x = to.x;
        p.y = to.y;
        world->addMarker(p);

        std::vector<const Cell*> currentCells = this->collectCells(from, to, world);
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
        p.type = ObjectType::CupBlue;
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
            if (ServiceRobot* robot = dynamic_cast<ServiceRobot*>(object)) {
                const Object* carriedObject = robot->getCarriedObject();
                if (carriedObject) {
                    Perception carryPercept;
                    carryPercept.objectID = carriedObject->getID();
                    carryPercept.state = ObjectState::Carried;
                    carryPercept.type = carriedObject->getType();
                    carryPercept.x = cellPerceptions.x;
                    carryPercept.y = cellPerceptions.y;
                    carryPercept.robotID = robot->getID();
//                    std::cout << "[ObjectDetection] Perceiving carried object " << carryPercept << std::endl;
                    cellPerceptions.perceptions.push_back(carryPercept);
                }
            }
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
    while(currentPoint != end) {
        // set next point
        pointStepX = Coordinate(currentPoint.x+sign_x,currentPoint.y);
        pointStepY = Coordinate(currentPoint.x,currentPoint.y+sign_y);
        double distanceStepX = geometry::distancePointToLineSegmentCalc(pointStepX.x, pointStepX.y, start.x, start.y, end.x, end.y);
        double distanceStepY = geometry::distancePointToLineSegmentCalc(pointStepY.x, pointStepY.y, start.x, start.y, end.x, end.y);
        if (distanceStepX < distanceStepY) {
            currentPoint = pointStepX;
        } else {
            currentPoint = pointStepY;
        }

        // check if sight is blocked in this cell
        const Cell* cell = world->getCell(currentPoint);
        if (!cell || cell->getType() == RoomType::Wall) {
            break;
        }
        bool sightBlocked = false;
        for (auto object : cell->getObjects()) {
            if (object->getType() == srgsim::ObjectType::Door && object->getState() == srgsim::ObjectState::Closed) {
                sightBlocked = true;
                break;
            }
        }
        if (sightBlocked) {
            break;
        }
        cells.push_back(cell);
    }
    return cells;
}
} // namespace srgsim