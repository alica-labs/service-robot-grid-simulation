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
    case containers::SimCommand::OPEN:
    case containers::SimCommand::CLOSE:
    case containers::SimCommand::PUTDOWN:
    case containers::SimCommand::PICKUP: {
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