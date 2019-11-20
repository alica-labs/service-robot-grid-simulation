#include "srg/sim/SimulatedRobot.h"

#include <srg/world/Cell.h>
#include <srg/world/ServiceRobot.h>

namespace srg
{
namespace sim
{
SimulatedRobot::SimulatedRobot(world::ServiceRobot* serviceRobot)
        : serviceRobot(serviceRobot)
{
    this->manipulation = new Arm(this);
    this->objectDetection = new Sensor(this);
}

world::Coordinate SimulatedRobot::getCoordinate()
{
    return dynamic_cast<const world::Cell*>(this->serviceRobot->getParentContainer())->coordinate;
}

world::Object* SimulatedRobot::getCarriedObject()
{
    if(this->serviceRobot->getObjects().size() > 0) {
        return this->serviceRobot->getObjects().begin()->second;
    } else {
        return nullptr;
    }
}

void SimulatedRobot::setCarriedObject(world::Object* object)
{
    this->serviceRobot->addObject(object);
}

essentials::IdentifierConstPtr SimulatedRobot::getID()
{
    return this->serviceRobot->getID();
}

containers::SimPerceptions SimulatedRobot::createSimPerceptions(Simulator* simulator)
{
    containers::SimPerceptions sps;
    sps.receiverID = this->getID();

    // objects
    std::vector<containers::CellPerception> objectPerceptions = this->objectDetection->createPerceptions(simulator);
    sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

    return sps;
}

void SimulatedRobot::executeAction(containers::SimCommand sc, World* world)
{
    this->manipulation->execute(sc, world);
}
} // namespace sim
} // namespace srg
