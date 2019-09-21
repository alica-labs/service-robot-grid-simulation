#pragma once

#include "CommandHandler.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace commands
{
class PutDownCommandHandler : public CommandHandler
{

public:
    PutDownCommandHandler(World* world): CommandHandler(world) {}
    ~PutDownCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
