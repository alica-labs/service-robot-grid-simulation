#pragma once

#include "srg/sim/containers/SimCommand.h"

#include <capnp/serialize-packed.h>

namespace srg
{
class Simulator;
namespace sim
{
namespace commands
{
class CommandHandler
{
public:
    CommandHandler(srg::Simulator* simulator);
    virtual ~CommandHandler() = default;
    virtual bool handle(srg::sim::containers::SimCommand sc) = 0;

protected:
    srg::Simulator* simulator;
};

} // namespace commands
} // namespace sim
} // namespace srg
