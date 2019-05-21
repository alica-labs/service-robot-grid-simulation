#include "srgsim/communication/CommandHandler.h"

#include "srgsim/SRGIDManager.h"

#include <srgsim/Simulator.h>

#include <essentials/ID.capnp.h>

namespace srgsim
{
namespace communication
{

const essentials::Identifier* CommandHandler::extractID(::capnp::FlatArrayMessageReader& msg)
{
    std::vector<uint8_t> idVector;
    idVector.assign(msg.getRoot<srgsim::Command>().getSenderId().getValue().begin(), msg.getRoot<srgsim::Command>().getSenderId().getValue().end());
    return this->simulator->getIdManager()->getIDFromBytes(idVector);
}

} // namespace communication
} // namespace srgsim