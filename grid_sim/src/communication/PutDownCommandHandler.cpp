#include "srgsim/communication/PutDownCommandHandler.h"

#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{

bool PutDownCommandHandler::handle(SimCommand sc)
{
    if (sc.action == SimCommand::PUTDOWN) {
        std::cout << "PutDownCommandHandler::handle() not implemented, yet!" << std::endl;
        return true;
    } else {
        return false;
    }
}
} // namespace communication
} // namespace srgsim