#pragma once

#include "srg/sim/commands/CommandHandler.h"

namespace srg
{
class World;
namespace sim
{
namespace commands
{
class SpawnCommandHandler : public CommandHandler
{
public:
    explicit SpawnCommandHandler(Simulator* simulator);
    ~SpawnCommandHandler() override = default;

    bool handle(containers::SimCommand sc);
};
} // namespace commands
} // namespace sim
} // namespace srg
