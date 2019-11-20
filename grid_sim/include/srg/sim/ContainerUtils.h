#pragma once

#include "srg/sim/containers/SimCommand.h"
#include "srg/sim/containers/SimPerceptions.h"
#include <srg/sim/msgs/SimPerceptionMsg.capnp.h>

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

    static void toMsg(containers::SimPerceptions simPerceptions, ::capnp::MallocMessageBuilder& builder);
    static containers::SimPerceptions toSimPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager);


private:
    ContainerUtils() = delete;
    static void toObjectListMsg(std::vector<srg::world::Object*>& objects, ::capnp::List<::srg::sim::SimPerceptionMsg::Object>::Builder& objectsListBuilder);
    static srg::world::Object* createObject(srg::sim::SimPerceptionMsg::Object::Reader& objectReader, essentials::IDManager* idManager);
};
} // namespace sim
} // namespace srg