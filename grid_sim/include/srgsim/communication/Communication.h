#pragma once

#include "srgsim/Command.capnp.h"

#include <capnp/serialize-packed.h>
#include <string>
#include <vector>

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

    capnzero::Subscriber* commandSubscriber;

private:
    static const std::string commandTopic;

    void commandCallback(::capnp::FlatArrayMessageReader& msg);

    void* ctx;
    std::string url;
    Simulator* simulator;
    std::vector<CommandHandler*> communicationHandlers;
};
} // namespace communication
} // namespace srgsim
