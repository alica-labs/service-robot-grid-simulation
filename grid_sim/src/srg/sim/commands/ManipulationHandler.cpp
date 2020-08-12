#include "srg/sim/commands/ManipulationHandler.h"

#include "srg/Simulator.h"
#include "srg/sim/SimulatedAgent.h"

#include <srg/World.h>

namespace srg
{
namespace sim
{
namespace commands
{
ManipulationHandler::ManipulationHandler(srg::Simulator* simulator)
        : CommandHandler(simulator)
{
}

bool ManipulationHandler::handle(containers::SimCommand sc)
{
    switch (sc.action) {
    case containers::Action::OPEN:
    case containers::Action::CLOSE:
    case containers::Action::PUTDOWN:
    case containers::Action::PICKUP: {
        srg::sim::SimulatedAgent* robot = simulator->getAgent(sc.senderID);
        if (!robot) {
            return false;
        }

        robot->executeAction(sc, simulator->getWorld());
        return true;
    }
    default:
        return false;
    }
}
} // namespace commands
} // namespace sim
} // namespace srg