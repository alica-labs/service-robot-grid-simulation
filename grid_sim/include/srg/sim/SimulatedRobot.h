#pragma once

#include "srg/sim/containers/SimPerceptions.h"
#include "srg/sim/Arm.h"
#include "srg/sim/Sensor.h"

#include <srg/world/ServiceRobot.h>

namespace srg
{
class Simulator;
namespace sim
{
class SimulatedRobot
{
public:
    world::Coordinate getCoordinate();
    world::Object* getCarriedObject();
    void setCarriedObject(world::Object* object);
    essentials::IdentifierConstPtr getID();
    containers::SimPerceptions createSimPerceptions(Simulator* simulator);
    void executeAction(containers::SimCommand sc, srg::World* world);

private:
    Sensor* objectDetection;
    Arm* manipulation;
    srg::world::ServiceRobot* serviceRobot;
};

} // namespace sim
} // namespace srg