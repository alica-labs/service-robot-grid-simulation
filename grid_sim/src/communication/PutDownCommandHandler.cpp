#include "srgsim/communication/PutDownCommandHandler.h"

#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{

bool PutDownCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{

    if (action != Command::Action::PUTDOWN)
        return false;

    // TODO implement
    return true;
}
} // namespace communication
} // namespace srgsim