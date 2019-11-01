#pragma once

#include "CommandHandler.h"

namespace srgsim
{
namespace commands
{
class ManipulationHandler : public CommandHandler
{

public:
    ManipulationHandler(World* world): CommandHandler(world) {}
    ~ManipulationHandler() override = default;

    bool handle(SimCommand sc) override;
};
} // namespace communication
} // namespace srgsim
