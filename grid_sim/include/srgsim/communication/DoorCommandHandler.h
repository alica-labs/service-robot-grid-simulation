#pragma once

#include "srgsim/communication/CommandHandler.h"

namespace srgsim
{
namespace communication
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
