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
    if (sc.action == containers::SimCommand::SPAWN) {
        simulator->addRobot(simulator->getWorld()->spawnRobot(sc.senderID));
        return true;
    } else {
        return false;
    }
}
} // namespace commands
} // namespace sim
} // namespace srg