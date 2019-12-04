#include "srg/sim/SimulatedAgent.h"

#include <srg/world/Cell.h>
#include <srg/world/Agent.h>

namespace srg
{
namespace sim
{
SimulatedAgent::SimulatedAgent(world::Agent* agent)
        : agent(agent)
{
    this->manipulation = new Arm(this);
    this->objectDetection = new Sensor(this);
}

world::Coordinate SimulatedAgent::getCoordinate()
{
    return dynamic_cast<const world::Cell*>(this->agent->getParentContainer())->coordinate;
}

world::Object* SimulatedAgent::getCarriedObject()
{
    if(this->agent->getObjects().size() > 0) {
        return this->agent->getObjects().begin()->second;
    } else {
        return nullptr;
    }
}

void SimulatedAgent::setCarriedObject(world::Object* object)
{
    this->agent->addObject(object);
}

essentials::IdentifierConstPtr SimulatedAgent::getID()
{
    return this->agent->getID();
}

containers::SimPerceptions SimulatedAgent::createSimPerceptions(Simulator* simulator)
{
    containers::SimPerceptions sps;
    sps.receiverID = this->getID();

    // objects
    std::vector<containers::CellPerception> objectPerceptions = this->objectDetection->createPerceptions(simulator);
    sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

    return sps;
}

void SimulatedAgent::executeAction(containers::SimCommand sc, World* world)
{
    this->manipulation->execute(sc, world);
}
} // namespace sim
} // namespace srg
