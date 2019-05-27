#include "srgsim/communication/PickUpCommandHandler.h"


#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{

bool PickUpCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{

    if (action != Command::Action::PICKUP)
        return false;

    // TODO implement
    return true;
}
} // namespace communication
} // namespace srgsim