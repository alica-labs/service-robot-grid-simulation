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
    PutDownCommandHandler(World* world): CommandHandler(world) {}
    ~PutDownCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
