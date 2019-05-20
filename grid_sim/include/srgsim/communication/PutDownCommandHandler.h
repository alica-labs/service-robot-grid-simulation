#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Command.capnp.h"
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

    bool handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg);

private:
    Simulator* simulator;
};
} // namespace communication
} // namespace srgsim
