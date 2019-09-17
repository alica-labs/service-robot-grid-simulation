#pragma once

#include "srgsim/SimCommandMsg.capnp.h"

#include <capnp/serialize-packed.h>
#include <string>
#include <vector>
#include <SystemConfig.h>

namespace capnzero
{
class Subscriber;
}

namespace srgsim
{
class Simulator;
namespace communication
{
class CommandHandler;
class Communication
{
public:
    Communication(Simulator* simulator);
    ~Communication();

private:
    void SimCommandCallback(::capnp::FlatArrayMessageReader& msg);

    essentials::SystemConfig* sc;
    void* ctx;
    std::string simCommandTopic;
    std::string address;
    capnzero::Subscriber* simCommandSub;

    Simulator* simulator;
    std::vector<CommandHandler*> communicationHandlers;
};
} // namespace communication
} // namespace srgsim
