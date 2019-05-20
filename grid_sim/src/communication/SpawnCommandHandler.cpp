#include "srgsim/communication/SpawnCommandHandler.h"

#include "srgsim/SRGIDManager.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{

bool SpawnCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{

    if (action != Command::Action::SPAWN)
        return false;

    const essentials::ID* id = this->extractID(msg);
    if (this->simulator->getIdManager()->addID(id)) {
        this->simulator->spawnRobot(id);
    }
    return true;
}
} // namespace communication
} // namespace srgsim