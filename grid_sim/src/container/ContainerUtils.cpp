#include "srgsim/containers/ContainerUtils.h"

#include <srgsim/SimCommandMsg.capnp.h>
#include <srgsim/SimPerceptionsMsg.capnp.h>
#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>

namespace srgsim
{
SimCommand ContainerUtils::toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    SimCommand sc;
    srgsim::SimCommandMsg::Reader reader = msg.getRoot<srgsim::SimCommandMsg>();
    sc.senderID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());

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
    SimCommandMsg::Builder msg = builder.getRoot<SimCommandMsg>();

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

SimPerceptions ContainerUtils::toSimPerceptions(::capnp::FlatArrayMessageReader &msg, essentials::IDManager *idManager) {
    SimPerceptions sps;
    srgsim::SimPerceptionsMsg::Reader reader = msg.getRoot<srgsim::SimPerceptionsMsg>();
    sps.receiverID = idManager->getIDFromBytes(
            reader.getReceiverID().getValue().asBytes().begin(), reader.getReceiverID().getValue().size(), reader.getReceiverID().getType());
    for (srgsim::SimPerceptionsMsg::Perception::Reader perceptionMsg : reader.getPerceptions()) {
        srgsim::Perception perception;
        perception.objectID = idManager->getIDFromBytes(
                perceptionMsg.getObjectID().getValue().asBytes().begin(), perceptionMsg.getObjectID().getValue().size(), perceptionMsg.getObjectID().getType());
        switch(perceptionMsg.getType()) {
            case srgsim::SimPerceptionsMsg::Perception::Type::ROBOT:
                perception.type = Type::Robot;
                break;
            case srgsim::SimPerceptionsMsg::Perception::Type::DOOR:
                perception.type = Type::Door;
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toSimPerceptions(): Unknown object type in capnp message found!" << std::endl;
                break;
        }
        perception.x = perceptionMsg.getX();
        perception.y = perceptionMsg.getY();
        sps.perceptions.push_back(perception);
    }
    return sps;
}

void ContainerUtils::toMsg(srgsim::SimPerceptions sp, ::capnp::MallocMessageBuilder &builder) {
    SimPerceptionsMsg::Builder msg = builder.getRoot<SimPerceptionsMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(sp.receiverID->getRaw(), (unsigned int) sp.receiverID->getSize()));
    receiverID.setType(sp.receiverID->getType());

    ::capnp::List< ::srgsim::SimPerceptionsMsg::Perception>::Builder perceptionsBuilder = msg.initPerceptions(sp.perceptions.size());
    for (unsigned int i = 0; i < sp.perceptions.size(); i++) {
        srgsim::SimPerceptionsMsg::Perception::Builder pBuilder = perceptionsBuilder[i];
        pBuilder.setY(sp.perceptions[i].y);
        pBuilder.setX(sp.perceptions[i].x);
        switch(sp.perceptions[i].type) {
            case Type::Robot:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::ROBOT);
                break;
            case Type::Door:
                pBuilder.setType(srgsim::SimPerceptionsMsg::Perception::Type::DOOR);
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toMsg(): Unknown object type perceived: " << sp.perceptions[i].type << "!" << std::endl;
                break;
        }
        capnzero::ID::Builder objectID = pBuilder.initObjectID();
        objectID.setType(sp.perceptions[i].objectID->getType());
        objectID.setValue(::capnp::Data::Reader(sp.perceptions[i].objectID->getRaw(), sp.perceptions[i].objectID->getSize()));
    }
}
} // namespace srgsim