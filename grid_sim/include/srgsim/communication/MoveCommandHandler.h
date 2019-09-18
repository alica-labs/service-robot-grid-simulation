#pragma once

#include "srgsim/communication/CommandHandler.h"

namespace srgsim
{
namespace communication
{
class MoveCommandHandler : public CommandHandler
{

public:
    MoveCommandHandler(World* world): CommandHandler(world) {}
    ~MoveCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
