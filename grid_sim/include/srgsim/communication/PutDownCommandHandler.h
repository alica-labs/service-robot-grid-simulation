#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{
class PutDownCommandHandler : public CommandHandler
{

public:
    PutDownCommandHandler(Simulator* simulator): CommandHandler(simulator) {}
    ~PutDownCommandHandler() = default;

    bool handle(SimCommand sc);

private:
    Simulator* simulator;
};
} // namespace communication
} // namespace srgsim
