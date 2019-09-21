#pragma once

#include "CommandHandler.h"

namespace srgsim
{
namespace commands
{
class DoorCommandHandler : public CommandHandler
{

public:
    DoorCommandHandler(World* world): CommandHandler(world) {}
    ~DoorCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
