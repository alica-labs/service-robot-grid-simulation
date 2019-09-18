#include "srgsim/containers/ContainerUtils.h"

#include <srgsim/SimCommandMsg.capnp.h>
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace srgsim
{
SimCommand ContainerUtils::toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{

    SimCommand sc;
    srgsim::SimCommandMsg::Reader reader = msg.getRoot<srgsim::SimCommandMsg>();
    essentials::WildcardID wildcardId(nullptr, 0);
    if (reader.getSenderID().getType() == essentials::Identifier::WILDCARD_TYPE) {
        sc.senderID = &wildcardId;
    } else {
        sc.senderID = idManager->getIDFromBytes(
                reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    }

    switch(reader.getAction()) {
        case srgsim::SimCommandMsg::Action::SPAWN:
            sc.action = SimCommand::SPAWN;
            break;
        case srgsim::SimCommandMsg::Action::CLOSE:
            sc.action = SimCommand::CLOSE;
            break;
        case srgsim::SimCommandMsg::Action::OPEN:
            sc.action = SimCommand::OPEN;
            break;
        case srgsim::SimCommandMsg::Action::PICKUP:
            sc.action = SimCommand::PICKUP;
            break;
        case srgsim::SimCommandMsg::Action::PUTDOWN:
            sc.action = SimCommand::PUTDOWN;
            break;
        case srgsim::SimCommandMsg::Action::GODOWN:
            sc.action = SimCommand::GODOWN;
            break;
        case srgsim::SimCommandMsg::Action::GOLEFT:
            sc.action = SimCommand::GOLEFT;
            break;
        case srgsim::SimCommandMsg::Action::GORIGHT:
            sc.action = SimCommand::GORIGHT;
            break;
        case srgsim::SimCommandMsg::Action::GOUP:
            sc.action = SimCommand::GOUP;
            break;
        default:
            std::cerr << "srgsim::ContainerUtils::toSimCommand(): Unknown action!" << std::endl;
    }

    sc.x = reader.getX();
    sc.y = reader.getY();

    return sc;
}

void ContainerUtils::toMsg(srgsim::SimCommand sc, ::capnp::MallocMessageBuilder& builder)
{
    srgsim::SimCommandMsg::Builder msg = builder.getRoot<srgsim::SimCommandMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(sc.senderID->getRaw(), (unsigned int) sc.senderID->getSize()));
    senderID.setType(sc.senderID->getType());

    switch(sc.action) {
        case SimCommand::SPAWN:
            msg.setAction(srgsim::SimCommandMsg::Action::SPAWN);
            break;
        case SimCommand::CLOSE:
            msg.setAction(srgsim::SimCommandMsg::Action::CLOSE);
            break;
        case SimCommand::OPEN:
            msg.setAction(srgsim::SimCommandMsg::Action::OPEN);
            break;
        case SimCommand::PICKUP:
            msg.setAction(srgsim::SimCommandMsg::Action::PICKUP);
            break;
        case SimCommand::PUTDOWN:
            msg.setAction(srgsim::SimCommandMsg::Action::PUTDOWN);
            break;
        case SimCommand::GODOWN:
            msg.setAction(srgsim::SimCommandMsg::Action::GODOWN);
            break;
        case SimCommand::GOLEFT:
            msg.setAction(srgsim::SimCommandMsg::Action::GOLEFT);
            break;
        case SimCommand::GORIGHT:
            msg.setAction(srgsim::SimCommandMsg::Action::GORIGHT);
            break;
        case SimCommand::GOUP:
            msg.setAction(srgsim::SimCommandMsg::Action::GOUP);
            break;
        default:
            std::cerr << "srgsim::ContainerUtils::toMsg(): Unknown action!" << std::endl;
    }

    msg.setX(sc.x);
    msg.setY(sc.y);
}
} // namespace srgsim