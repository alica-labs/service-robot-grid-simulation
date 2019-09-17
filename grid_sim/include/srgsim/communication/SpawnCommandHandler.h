#pragma once

#include "srgsim/communication/CommandHandler.h"
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

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
