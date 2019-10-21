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
            this->world->openDoor(sc.objectID);
            return true;
        case SimCommand::CLOSE:
            this->world->closeDoor(sc.objectID);
            return true;
        default:
            return false;
    }
}
} // namespace communication
} // namespace srgsim