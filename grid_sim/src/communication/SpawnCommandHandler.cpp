#include "srgsim/communication/SpawnCommandHandler.h"

#include "srgsim/Simulator.h"

#include <capnzero/ID.capnp.h>
#include <essentials/IDManager.h>

namespace srgsim
{
namespace communication
{

bool SpawnCommandHandler::handle(SimCommand sc)
{
    if (sc.action == SimCommand::SPAWN) {
        this->simulator->spawnRobot(sc.senderID);
        return true;
    } else {
        return false;
    }
}
} // namespace communication
} // namespace srgsim