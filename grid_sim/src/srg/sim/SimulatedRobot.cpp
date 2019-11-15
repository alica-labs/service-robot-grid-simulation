#include "srg/sim/SimulatedRobot.h"

#include <srg/world/Cell.h>

namespace srg
{
namespace sim
{

world::Coordinate SimulatedRobot::getCoordinate()
{
    return this->serviceRobot->getCell()->coordinate;
}

world::Object* SimulatedRobot::getCarriedObject()
{
    return this->serviceRobot->getCarriedObject();
}

void SimulatedRobot::setCarriedObject(world::Object* object) {
    this->serviceRobot->setCarriedObject(object);
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
    std::vector<containers::CellPerceptions> objectPerceptions = this->objectDetection->createPerceptions(simulator);
    sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

    return sps;
}

void SimulatedRobot::executeAction(containers::SimCommand sc, World* world)
{
    this->manipulation->execute(sc, world);
}
} // namespace sim
} // namespace srg
