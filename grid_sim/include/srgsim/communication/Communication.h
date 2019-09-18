#pragma once

#include "srgsim/SimCommandMsg.capnp.h"
#include "srgsim/containers/SimPerceptions.h"

#include <SystemConfig.h>
#include <essentials/IDManager.h>

#include <capnp/serialize-packed.h>
#include <string>
#include <vector>

namespace capnzero
{
class Subscriber;
class Publisher;
}

namespace srgsim
{
class World;
namespace communication
{
class CommandHandler;
class Communication
{
public:
    Communication(essentials::IDManager* idManager, World* world);
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

    World* world;
    essentials::IDManager* idManager;
    std::vector<CommandHandler*> communicationHandlers;
};
} // namespace communication
} // namespace srgsim
