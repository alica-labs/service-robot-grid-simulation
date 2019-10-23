#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/CellPerceptions.h"

#include <SystemConfig.h>

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
        int32_t xDelta = sin(currentDegree) * sightLimit;
        int32_t yDelta = cos(currentDegree) * sightLimit;
//        std::cout << "ObjectDetection::createPerceptions(): xDelta " << xDelta << " yDelta " << yDelta << std::endl;

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
        p.type = Type::Unknown;
        p.x = entry.second->coordinate.x;
        p.y = entry.second->coordinate.y;
        world->addMarker(p);

        const std::vector<Object*>& objects = entry.second->getObjects();
        CellPerceptions cellPerceptions;
        cellPerceptions.x = entry.second->coordinate.x;
        cellPerceptions.y = entry.second->coordinate.y;
        for (Object* object : objects) {
            Perception p;
            p.type = object->getType();
            p.objectID = object->getID();
            p.x = cellPerceptions.x;
            p.y = cellPerceptions.y;
            cellPerceptions.perceptions.push_back(p);
        }
    }
    std::cout << "ObjectDetection::createPerceptions(): --------------------- " << std::endl;

    return cellPerceptionsList;
}

std::vector<const Cell*> ObjectDetection::collectCells(Coordinate p0, Coordinate p1, World* world)
{
    int32_t dx = p1.x - p0.x;
    int32_t dy = p1.y - p0.y;
    uint32_t nx = abs(dx);
    uint32_t ny = abs(dy);
    int32_t sign_x = dx > 0 ? 1 : -1;
    int32_t sign_y = dy > 0 ? 1 : -1;

    std::vector<const Cell*> cells;
    Coordinate p(p0.x, p0.y);
    cells.push_back(world->getCell(p));
    for (uint32_t ix = 0, iy = 0; ix < nx || iy < ny;) {
        if (nx == 0) {
            // next step is vertical
            p.y += sign_y;
            iy++;
        } else if (ny == 0) {
            // next step is horizontal
            p.x += sign_x;
            ix++;
        } else if ((ix + 1) / nx < (iy + 1) / ny) {
            // next step is horizontal
            p.x += sign_x;
            ix++;
        } else {
            // next step is vertical
            p.y += sign_y;
            iy++;
        }
        const Cell* cell = world->getCell(p);
        if (cell && cell->type == Type::Floor) {
            cells.push_back(world->getCell(p));
        } else {
            break;
        }
    }
    return cells;
}
} // namespace srgsim