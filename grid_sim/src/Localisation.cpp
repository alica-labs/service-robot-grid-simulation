#include "srgsim/Localisation.h"
#include "srgsim/Cell.h"
#include "srgsim/SRGEnums.h"
#include "srgsim/ServiceRobot.h"

namespace srgsim
{
Localisation::Localisation(srgsim::ServiceRobot* robot)
        : Sensor(robot)
{
}

std::vector<Perception> Localisation::createPerceptions(World* world)
{
    std::vector<Perception> ps;

    Perception p;
    p.type = Type::Robot;
    p.objectID = this->robot->getID();
    p.x = this->robot->getCell()->coordinate.x;
    p.y = this->robot->getCell()->coordinate.y;
    ps.push_back(p);

    return ps;
}
} // namespace srgsim