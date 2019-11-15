#pragma once

#include "srg/sim/commands/CommandHandler.h"

namespace srg
{
namespace sim
{
namespace commands
{
class ManipulationHandler : public CommandHandler
{

public:
    ManipulationHandler(Simulator* simulator);
    ~ManipulationHandler() override = default;
    bool handle(containers::SimCommand sc) override;
};
} // namespace commands
} // namespace sim
} // namespace srg
