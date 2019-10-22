#include "srgsim/world/ObjectDetection.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/world/World.h"

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

std::vector<Perception> ObjectDetection::createPerceptions(World* world)
{
    // collect cells in vision
    Coordinate ownCoord = this->robot->getCell()->coordinate;
    std::map<Coordinate, const Cell*> cellsInVision;
    for (double currentDegree = -M_PI; currentDegree < M_PI; currentDegree += M_PI / 90) { // PI/90 <=> 2 degree resolution
        int32_t xDelta = sin(currentDegree) * sightLimit;
        int32_t yDelta = cos(currentDegree) * sightLimit;
        std::cout << "ObjectDetection::createPerceptions(): xDelta " << xDelta << " yDelta " << yDelta << std::endl;

        std::vector<const Cell*> currentCells = this->collectCells(ownCoord, Coordinate(ownCoord.x + xDelta, ownCoord.y + yDelta), world);
        for (const Cell* cell : currentCells) {
            // add only cells that are not already collected
            if (cellsInVision.find(cell->coordinate) == cellsInVision.end()) {
                cellsInVision.emplace(cell->coordinate, cell);
            }
        }
    }

    // collect objects as perceptions
    std::vector<Perception> ps;
    for (auto& entry : cellsInVision) {
        for (Object* object : entry.second->getObjects()) {
            Perception p;
            p.type = object->getType();
            p.objectID = object->getID();
            p.x = entry.second->coordinate.x;
            p.y = entry.second->coordinate.y;
            ps.push_back(p);
        }
    }

    return ps;
}

std::vector<const Cell*> ObjectDetection::collectCells(Coordinate p0, Coordinate p1, World* world)
{
    double dx = p1.x - p0.x;
    double dy = p1.y - p0.y;

    double nx = abs(dx);
    double ny = abs(dy);
    double sign_x = dx > 0 ? 1 : -1, sign_y = dy > 0 ? 1 : -1;

    Coordinate p(p0.x, p0.y);
    std::vector<const Cell*> cells;
    cells.push_back(world->getCell(Coordinate(p.x, p.y)));
    for (uint32_t ix = 0, iy = 0; ix < nx || iy < ny;) {
        if ((0.5 + ix) / nx < (0.5 + iy) / ny) {
            // next step is horizontal
            p.x += sign_x;
            ix++;
        } else {
            // next step is vertical
            p.y += sign_y;
            iy++;
        }
        const Cell* cell = world->getCell(Coordinate(p.x, p.y));
        if (cell->type == Type::Floor) {
            cells.push_back(world->getCell(Coordinate(p.x, p.y)));
        } else {
            break;
        }
    }
    return cells;
}
} // namespace srgsim