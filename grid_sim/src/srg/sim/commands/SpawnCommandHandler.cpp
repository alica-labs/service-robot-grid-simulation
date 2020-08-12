#include "srg/sim/commands/SpawnCommandHandler.h"

#include "srg/Simulator.h"

#include <srg/World.h>

namespace srg
{
namespace sim
{
namespace commands
{
SpawnCommandHandler::SpawnCommandHandler(srg::Simulator* simulator)
        : CommandHandler(simulator)
{
}

bool SpawnCommandHandler::handle(containers::SimCommand sc)
{
    if (sc.action == containers::Action::SPAWNROBOT) {
        simulator->addSimulatedAgent(simulator->getWorld()->spawnAgent(sc.senderID, srg::world::ObjectType::Robot));
        return true;
    } else if (sc.action == containers::Action::SPAWNHUMAN) {
        simulator->addSimulatedAgent(simulator->getWorld()->spawnAgent(sc.senderID, srg::world::ObjectType::Human));
        return true;
    } else {
        return false;
    }
}
} // namespace commands
} // namespace sim
} // namespace srg