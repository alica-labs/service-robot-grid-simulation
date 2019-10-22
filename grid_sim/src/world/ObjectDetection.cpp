#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/Cell.h"
#include "srgsim/world/World.h"
#include "srgsim/containers/Coordinate.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/world/ServiceRobot.h"

#include <SystemConfig.h>

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
//        std::cout << "ObjectDetection::createPerceptions(): Not yet implemented!" << std::endl;
        std::vector<Perception> ps;

        Perception p;
        p.type = Type::CupYellow;
        p.objectID = this->robot->getID();
        p.x = this->robot->getCell()->coordinate.x;
        p.y = this->robot->getCell()->coordinate.y;
        ps.push_back(p);

        return ps;
    }

    std::vector<const Cell*> collectCells(Coordinate p0, Coordinate p1, World* world) {
        double dx = p1.x-p0.x;
        double dy = p1.y-p0.y;

        double nx = abs(dx);
        double ny = abs(dy);
        double sign_x = dx > 0? 1 : -1, sign_y = dy > 0? 1 : -1;

        Coordinate p(p0.x, p0.y);
        std::vector<const Cell*> cells;
        cells.push_back(world->getCell(Coordinate(p.x, p.y)));
        for (uint32_t ix = 0, iy = 0; ix < nx || iy < ny;) {
            if ((0.5+ix) / nx < (0.5+iy) / ny) {
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