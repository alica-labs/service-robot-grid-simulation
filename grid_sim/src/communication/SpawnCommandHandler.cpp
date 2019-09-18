#include "srgsim/communication/SpawnCommandHandler.h"

#include "srgsim/Simulator.h"

#include <capnzero/ID.capnp.h>
#include <essentials/IDManager.h>

namespace srgsim
{
namespace communication
{

bool SpawnCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{
    if (action != Command::Action::SPAWN)
        return false;

    std::cout << "recieved spawn command!\n";

    capnzero::ID::Reader idReader = msg.getRoot<srgsim::Command>().getSenderId();
    std::cout << "spawning robot" << std::endl;
    std::cout << idReader.toString().flatten().cStr() << std::endl;
    std::cout << "idManager " << this->simulator->getIdManager() << std::endl;
    this->simulator->spawnRobot(this->simulator->getIdManager()->getIDFromBytes(
            idReader.getValue().asBytes().begin(), idReader.getValue().size(), static_cast<uint8_t>(idReader.getType())));
    std::cout << "returning" << std::endl;
    return true;
}
} // namespace communication
} // namespace srgsim