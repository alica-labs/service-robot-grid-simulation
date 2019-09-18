#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{
class MoveCommandHandler : public CommandHandler
{

public:
    MoveCommandHandler(Simulator* simulator): CommandHandler(simulator) {}
    ~MoveCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
