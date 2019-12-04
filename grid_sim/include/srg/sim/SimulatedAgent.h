#pragma once

#include "srg/sim/Arm.h"
#include "srg/sim/Sensor.h"
#include "srg/sim/containers/SimPerceptions.h"

namespace srg
{
class Simulator;
namespace world
{
class Agent;
}
namespace sim
{
class SimulatedAgent
{
public:
    SimulatedAgent(srg::world::Agent* agent);
    world::Coordinate getCoordinate();
    world::Object* getCarriedObject();
    void setCarriedObject(world::Object* object);
    essentials::IdentifierConstPtr getID();
    containers::SimPerceptions createSimPerceptions(Simulator* simulator);
    void executeAction(containers::SimCommand sc, srg::World* world);

private:
    Sensor* objectDetection;
    Arm* manipulation;
    srg::world::Agent* agent;
};

} // namespace sim
} // namespace srg