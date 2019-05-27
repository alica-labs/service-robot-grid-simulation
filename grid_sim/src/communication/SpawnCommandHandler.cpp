#include "srgsim/communication/SpawnCommandHandler.h"

#include "srgsim/Simulator.h"

#include <essentials/IDManager.h>
#include <capnzero/ID.capnp.h>

namespace srgsim
{
namespace communication
{

bool SpawnCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{
    if (action != Command::Action::SPAWN)
        return false;

    ::capnp::Data::Reader idReader = msg.getRoot<srgsim::Command>().getSenderId().getValue();
    this->simulator->spawnRobot(this->simulator->getIdManager()->getIDFromBytes(idReader.asBytes().begin(), idReader.size()));
    return true;
}
} // namespace communication
} // namespace srgsim