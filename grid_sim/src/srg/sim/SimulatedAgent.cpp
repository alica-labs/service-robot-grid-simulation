#include "srg/sim/SimulatedAgent.h"

#include <srg/world/Cell.h>
#include <srg/world/Agent.h>

namespace srg
{
namespace sim
{
SimulatedAgent::SimulatedAgent(std::shared_ptr<world::Agent> agent)
        : agent(agent)
{
    this->manipulation = new Arm(this);
    this->objectDetection = new Sensor(this);
}

world::Coordinate SimulatedAgent::getCoordinate()
{
    return std::dynamic_pointer_cast<const world::Cell>(this->agent->getParentContainer())->coordinate;
}

std::shared_ptr<world::Object> SimulatedAgent::getCarriedObject()
{
    if(this->agent->getObjects().size() > 0) {
        return this->agent->getObjects().begin()->second;
    } else {
        return nullptr;
    }
}

void SimulatedAgent::setCarriedObject(std::shared_ptr<world::Object> object)
{
    this->agent->addObject(object);
}

essentials::IdentifierConstPtr SimulatedAgent::getID()
{
    return this->agent->getID();
}

containers::Perceptions SimulatedAgent::createSimPerceptions(Simulator* simulator)
{
    containers::Perceptions sps;
    sps.receiverID = this->getID();
    sps.timestamp = std::chrono::system_clock::now().time_since_epoch();

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
