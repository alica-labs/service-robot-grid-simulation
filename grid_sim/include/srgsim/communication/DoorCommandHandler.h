#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{
class DoorCommandHandler : public CommandHandler
{

public:
    DoorCommandHandler(Simulator* simulator): CommandHandler(simulator) {}
    ~DoorCommandHandler() = default;

    bool handle(SimCommand sc);

private:
    Simulator* simulator;
};
} // namespace communication
} // namespace srgsim
