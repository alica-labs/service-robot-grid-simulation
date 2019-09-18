#pragma once

#include "srgsim/communication/CommandHandler.h"
#include "srgsim/Simulator.h"

namespace srgsim
{
namespace communication
{
class PickUpCommandHandler : public CommandHandler
{

public:
    PickUpCommandHandler(Simulator* simulator): CommandHandler(simulator) {}
    ~PickUpCommandHandler() = default;

    bool handle(SimCommand sc);
};
} // namespace communication
} // namespace srgsim
