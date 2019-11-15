#pragma once

#include "srg/sim/containers/SimCommand.h"

#include <essentials/IdentifierConstPtr.h>

#include <vector>

namespace srg
{
class World;
namespace sim
{
class SimulatedRobot;
class Arm
{
public:
    Arm(SimulatedRobot* robot);
    bool execute(containers::SimCommand sc, srg::World* world);

private:
    bool open(essentials::IdentifierConstPtr objectID, srg::World* world);
    bool close(essentials::IdentifierConstPtr objectID, srg::World* world);
    bool pickUp(essentials::IdentifierConstPtr objectID, srg::World* world);
    bool putDown(containers::SimCommand sc, srg::World* world);
    SimulatedRobot* robot;
};
} // namespace sim
} // namespace srg