#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Command.capnp.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{
class SpawnCommandHandler : public CommandHandler
{

public:
    SpawnCommandHandler(Simulator* simulator): CommandHandler(simulator) {}
    ~SpawnCommandHandler() = default;

    bool handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg);

private:
    Simulator* simulator;
};
} // namespace communication
} // namespace srgsim
