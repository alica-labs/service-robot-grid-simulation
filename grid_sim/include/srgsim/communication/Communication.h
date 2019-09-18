#pragma once

#include "srgsim/SimCommandMsg.capnp.h"
#include "srgsim/containers/SimPerceptions.h"

#include <capnp/serialize-packed.h>
#include <string>
#include <vector>
#include <SystemConfig.h>

namespace capnzero
{
class Subscriber;
class Publisher;
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

    void sendSimPerceptions(SimPerceptions sp);

private:
    void SimCommandCallback(::capnp::FlatArrayMessageReader& msg);

    essentials::SystemConfig* sc;
    void* ctx;
    std::string simCommandTopic;
    std::string simPerceptionsTopic;
    std::string address;
    capnzero::Subscriber* simCommandSub;
    capnzero::Publisher* simPerceptionsPub;

    Simulator* simulator;
    std::vector<CommandHandler*> communicationHandlers;
};
} // namespace communication
} // namespace srgsim
