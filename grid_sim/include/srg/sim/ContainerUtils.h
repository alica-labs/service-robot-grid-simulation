#pragma once

#include "srg/sim/containers/SimCommand.h"
#include "srg/sim/containers/Perceptions.h"

#include <srg/sim/msgs/PerceptionMsg.capnp.h>

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

    static void toMsg(containers::Perceptions simPerceptions, ::capnp::MallocMessageBuilder& builder);
    static containers::Perceptions toPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);
    static containers::Perceptions createPerceptions(srg::sim::PerceptionMsg::Reader perceptionsReader, essentials::IDManager* idManager);
    static void toMsg(containers::Perceptions perceptions, ::srg::sim::PerceptionMsg::Builder& builder);

private:
    ContainerUtils() = delete;
    static void toObjectListMsg(std::vector<std::shared_ptr<srg::world::Object>>& objects, ::capnp::List<::srg::sim::PerceptionMsg::Object>::Builder& objectsListBuilder);
    static std::shared_ptr<srg::world::Object> createObject(srg::sim::PerceptionMsg::Object::Reader& objectReader, essentials::IDManager* idManager);
};
} // namespace sim
} // namespace srg