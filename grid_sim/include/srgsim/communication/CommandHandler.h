#pragma once

#include "srgsim/SimCommandMsg.capnp.h"
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
    virtual bool handle(SimCommand sc) = 0;
protected:
    Simulator* simulator;
};

} // namespace communication
} // namespace srgsim

