#include "srgsim/communication/DoorCommandHandler.h"

#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{

bool DoorCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{

    if (action != Command::Action::OPEN && action != Command::Action::CLOSE)
        return false;

    // TODO implement
    return true;
}
} // namespace communication
} // namespace srgsim