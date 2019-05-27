#include "srgsim/communication/MoveCommandHandler.h"

#include "srgsim/Simulator.h"

#include <essentials/IDManager.h>

namespace srgsim
{
namespace communication
{

bool MoveCommandHandler::handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg)
{
    if (action != Command::Action::GODOWN && action != Command::Action::GOUP && action != Command::Action::GORIGHT && action != Command::Action::GOLEFT)
        return false;

    capnzero::ID::Reader idReader = msg.getRoot<srgsim::Command>().getSenderId();
    const essentials::Identifier* id =
            this->simulator->getIdManager()->getIDFromBytes(idReader.getValue().asBytes().begin(), idReader.getValue().size(),
                                                            static_cast<uint8_t>(idReader.getType()));
    switch (action) {
    case Command::Action::GOLEFT:
        this->simulator->moveObject(id, Direction::Left);
        return true;
    case Command::Action::GOUP:
        this->simulator->moveObject(id, Direction::Up);
        return true;
    case Command::Action::GORIGHT:
        this->simulator->moveObject(id, Direction::Right);
        return true;
    case Command::Action::GODOWN:
        this->simulator->moveObject(id, Direction::Down);
        return true;
    default:
        std::cout << "MoveCommandHandler: unreachable case." << std::endl;
        return false;
    }
}
} // namespace communication
} // namespace srgsim