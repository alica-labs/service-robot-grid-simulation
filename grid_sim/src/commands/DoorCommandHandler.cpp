#include "srgsim/commands/DoorCommandHandler.h"

#include "srgsim/Simulator.h"

namespace srgsim
{
namespace commands
{

bool DoorCommandHandler::handle(SimCommand sc)
{
    switch(sc.action) {
        case SimCommand::OPEN:
            std::cout << "DoorCommandHandler::handle() not implemented, yet!" << std::endl;
            return true;
        case SimCommand::CLOSE:
            std::cout << "DoorCommandHandler::handle() not implemented, yet!" << std::endl;
            return true;
        default:
            return false;
    }
}
} // namespace communication
} // namespace srgsim