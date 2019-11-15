#pragma once

#include "srg/sim/containers/SimCommand.h"
#include "srg/sim/containers/SimPerceptions.h"

#include <capnzero/CapnZero.h>

namespace essentials
{
class IDManager;
}

namespace srg
{
namespace sim
{
class ContainerUtils
{
public:
    static containers::SimCommand toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);

    static void toMsg(containers::SimCommand simCommand, ::capnp::MallocMessageBuilder& builder);

    static containers::SimPerceptions toSimPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);

    static void toMsg(containers::SimPerceptions simPerceptions, ::capnp::MallocMessageBuilder& builder);

private:
    ContainerUtils() = delete;
};
} // namespace sim
} // namespace srg