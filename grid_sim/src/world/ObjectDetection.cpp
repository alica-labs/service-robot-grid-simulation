#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/Cell.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/world/ServiceRobot.h"

namespace srgsim
{
    ObjectDetection::ObjectDetection(srgsim::ServiceRobot* robot)
            : Sensor(robot)
    {
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
} // namespace srgsim