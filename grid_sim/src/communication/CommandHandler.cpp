#include "srgsim/communication/CommandHandler.h"

#include "srgsim/SRGIDManager.h"

#include <srgsim/Simulator.h>

namespace srgsim
{
namespace communication
{

const essentials::ID* CommandHandler::extractID(::capnp::FlatArrayMessageReader& msg)
{
    std::vector<uint8_t> idVector;
    idVector.assign(msg.getRoot<srgsim::Command>().getSenderId().begin(), msg.getRoot<srgsim::Command>().getSenderId().end());
    return this->simulator->getIdManager()->getIDFromBytes(idVector);
}

} // namespace communication
} // namespace srgsim