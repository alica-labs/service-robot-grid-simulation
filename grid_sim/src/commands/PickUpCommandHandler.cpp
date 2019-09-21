#include "srgsim/commands/PickUpCommandHandler.h"


#include "srgsim/Simulator.h"

namespace srgsim
{
namespace commands
{

bool PickUpCommandHandler::handle(SimCommand sc)
{
    if (sc.action == SimCommand::PICKUP) {
        std::cout << "PickUpCommandHandler::handle() not implemented, yet!" << std::endl;
        return true;
    } else {
        return false;
    }
}
} // namespace communication
} // namespace srgsim