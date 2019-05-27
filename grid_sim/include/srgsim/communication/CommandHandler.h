#pragma once

#include "srgsim/Command.capnp.h"
#include "srgsim/Simulator.h"
#include <capnp/serialize-packed.h>

namespace srgsim
{
namespace communication
{

class CommandHandler
{

public:
    CommandHandler(Simulator* simulator): simulator(simulator){};
    virtual ~CommandHandler() {}
    virtual bool handle(Command::Action action, ::capnp::FlatArrayMessageReader& msg) = 0;

protected:
    Simulator* simulator;
};

} // namespace communication
} // namespace srgsim
