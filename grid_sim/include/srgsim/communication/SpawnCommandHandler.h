#pragma once

#include "srgsim/communication/CommandHandler.h"

namespace srgsim
{
namespace communication
{
class SpawnCommandHandler : public CommandHandler
{

public:
    SpawnCommandHandler(World* world): CommandHandler(world) {}
    ~SpawnCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
