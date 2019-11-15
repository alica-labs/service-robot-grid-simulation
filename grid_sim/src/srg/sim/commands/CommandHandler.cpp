#include "srg/sim/commands/CommandHandler.h"

#include "srg/Simulator.h"

namespace srg
{
namespace sim
{
namespace commands
{
CommandHandler::CommandHandler(srg::Simulator* simulator)
        : simulator(simulator)
{
}
} // namespace commands
} // namespace sim
} // namespace srg
