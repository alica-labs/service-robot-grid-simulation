#include "srgsim/communication/MoveCommandHandler.h"

#include "srgsim/Simulator.h"

#include <essentials/IDManager.h>

namespace srgsim
{
namespace communication
{

bool MoveCommandHandler::handle(SimCommand sc)
{
    switch (sc.action) {
    case SimCommand::GOLEFT:
        this->simulator->moveObject(sc.senderID, Direction::Left);
        return true;
    case SimCommand::GOUP:
        this->simulator->moveObject(sc.senderID, Direction::Up);
        return true;
    case SimCommand::GORIGHT:
        this->simulator->moveObject(sc.senderID, Direction::Right);
        return true;
    case SimCommand::GODOWN:
        this->simulator->moveObject(sc.senderID, Direction::Down);
        return true;
    default:
        return false;
    }
}
} // namespace communication
} // namespace srgsim