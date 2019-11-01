#pragma once

#include "srgsim/containers/SimCommand.h"
#include "srgsim/world/World.h"
#include <capnp/serialize-packed.h>

namespace srgsim
{
namespace commands
{

class CommandHandler
{

public:
    explicit CommandHandler(World* world): world(world){};
    virtual ~CommandHandler() = default;
    virtual bool handle(SimCommand sc) = 0;
protected:
    World* world;
};

} // namespace communication
} // namespace srgsim

