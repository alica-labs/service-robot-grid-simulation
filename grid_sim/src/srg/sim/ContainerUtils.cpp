#include "srg/sim/ContainerUtils.h"

#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>
#include <srg/sim/msgs/SimCommandMsg.capnp.h>
#include <srg/sim/msgs/SimPerceptionsMsg.capnp.h>

namespace srg
{
namespace sim
{
containers::SimCommand ContainerUtils::toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    containers::SimCommand sc;
    srg::sim::SimCommandMsg::Reader reader = msg.getRoot<srg::sim::SimCommandMsg>();
    sc.senderID = idManager->getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    sc.objectID = idManager->getIDFromBytes(
            reader.getObjectID().getValue().asBytes().begin(), reader.getObjectID().getValue().size(), reader.getObjectID().getType());

    switch (reader.getAction()) {
    case srg::sim::SimCommandMsg::Action::SPAWN:
        sc.action = containers::SimCommand::SPAWN;
        break;
    case srg::sim::SimCommandMsg::Action::CLOSE:
        sc.action = containers::SimCommand::CLOSE;
        break;
    case srg::sim::SimCommandMsg::Action::OPEN:
        sc.action = containers::SimCommand::OPEN;
        break;
    case srg::sim::SimCommandMsg::Action::PICKUP:
        sc.action = containers::SimCommand::PICKUP;
        break;
    case srg::sim::SimCommandMsg::Action::PUTDOWN:
        sc.action = containers::SimCommand::PUTDOWN;
        break;
    case srg::sim::SimCommandMsg::Action::GODOWN:
        sc.action = containers::SimCommand::GODOWN;
        break;
    case srg::sim::SimCommandMsg::Action::GOLEFT:
        sc.action = containers::SimCommand::GOLEFT;
        break;
    case srg::sim::SimCommandMsg::Action::GORIGHT:
        sc.action = containers::SimCommand::GORIGHT;
        break;
    case srg::sim::SimCommandMsg::Action::GOUP:
        sc.action = containers::SimCommand::GOUP;
        break;
    default:
        std::cerr << "srgsim::ContainerUtils::toSimCommand(): Unknown action!" << std::endl;
    }

    sc.x = reader.getX();
    sc.y = reader.getY();

    return sc;
}

void ContainerUtils::toMsg(srg::sim::containers::SimCommand sc, ::capnp::MallocMessageBuilder& builder)
{
    SimCommandMsg::Builder msg = builder.getRoot<SimCommandMsg>();

    capnzero::ID::Builder senderID = msg.initSenderID();
    senderID.setValue(kj::arrayPtr(sc.senderID->getRaw(), (unsigned int) sc.senderID->getSize()));
    senderID.setType(sc.senderID->getType());

    capnzero::ID::Builder objectID = msg.initObjectID();
    objectID.setValue(kj::arrayPtr(sc.objectID->getRaw(), (unsigned int) sc.objectID->getSize()));
    objectID.setType(sc.objectID->getType());

    switch (sc.action) {
    case containers::SimCommand::SPAWN:
        msg.setAction(srg::sim::SimCommandMsg::Action::SPAWN);
        break;
    case containers::SimCommand::CLOSE:
        msg.setAction(srg::sim::SimCommandMsg::Action::CLOSE);
        break;
    case containers::SimCommand::OPEN:
        msg.setAction(srg::sim::SimCommandMsg::Action::OPEN);
        break;
    case containers::SimCommand::PICKUP:
        msg.setAction(srg::sim::SimCommandMsg::Action::PICKUP);
        break;
    case containers::SimCommand::PUTDOWN:
        msg.setAction(srg::sim::SimCommandMsg::Action::PUTDOWN);
        break;
    case containers::SimCommand::GODOWN:
        msg.setAction(srg::sim::SimCommandMsg::Action::GODOWN);
        break;
    case containers::SimCommand::GOLEFT:
        msg.setAction(srg::sim::SimCommandMsg::Action::GOLEFT);
        break;
    case containers::SimCommand::GORIGHT:
        msg.setAction(srg::sim::SimCommandMsg::Action::GORIGHT);
        break;
    case containers::SimCommand::GOUP:
        msg.setAction(srg::sim::SimCommandMsg::Action::GOUP);
        break;
    default:
        std::cerr << "srgsim::ContainerUtils::toMsg(): Unknown action!" << std::endl;
    }

    msg.setX(sc.x);
    msg.setY(sc.y);
}

containers::SimPerceptions ContainerUtils::toSimPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager* idManager)
{
    containers::SimPerceptions sps;
    srg::sim::SimPerceptionsMsg::Reader reader = msg.getRoot<srg::sim::SimPerceptionsMsg>();
    sps.receiverID = idManager->getIDFromBytes(
            reader.getReceiverID().getValue().asBytes().begin(), reader.getReceiverID().getValue().size(), reader.getReceiverID().getType());
    for (srg::sim::SimPerceptionsMsg::CellPerceptions::Reader cellPerceptionsMsg : reader.getCellPerceptions()) {
        srg::sim::containers::CellPerceptions cellPerceptions;
        cellPerceptions.x = cellPerceptionsMsg.getX();
        cellPerceptions.y = cellPerceptionsMsg.getY();

        for (srg::sim::SimPerceptionsMsg::Perception::Reader perceptionMsg : cellPerceptionsMsg.getPerceptions()) {
            srg::sim::containers::Perception perception;
            perception.objectID = idManager->getIDFromBytes(perceptionMsg.getObjectID().getValue().asBytes().begin(),
                    perceptionMsg.getObjectID().getValue().size(), perceptionMsg.getObjectID().getType());
            perception.robotID = idManager->getIDFromBytes(perceptionMsg.getRobotID().getValue().asBytes().begin(),
                    perceptionMsg.getRobotID().getValue().size(), perceptionMsg.getRobotID().getType());
            perception.x = perceptionMsg.getX();
            perception.y = perceptionMsg.getY();
            switch (perceptionMsg.getType()) {
            case srg::sim::SimPerceptionsMsg::Perception::Type::ROBOT:
                perception.type = srg::world::ObjectType::Robot;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::Type::DOOR:
                perception.type = srg::world::ObjectType::Door;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::Type::CUPRED:
                perception.type = srg::world::ObjectType::CupRed;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::Type::CUPBLUE:
                perception.type = srg::world::ObjectType::CupBlue;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::Type::CUPYELLOW:
                perception.type = srg::world::ObjectType::CupYellow;
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toSimPerceptions(): Unknown object type in capnp message found!" << std::endl;
                break;
            }
            switch (perceptionMsg.getState()) {
            case srg::sim::SimPerceptionsMsg::Perception::State::OPEN:
                perception.state = srg::world::ObjectState::Open;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::State::CLOSED:
                perception.state = srg::world::ObjectState::Closed;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::State::CARRIED:
                perception.state = srg::world::ObjectState::Carried;
                break;
            case srg::sim::SimPerceptionsMsg::Perception::State::UNDEFINED:
                perception.state = srg::world::ObjectState::Undefined;
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toSimPerceptions(): Unknown object type in capnp message found!" << std::endl;
                break;
            }
            cellPerceptions.perceptions.push_back(perception);
        }
        sps.cellPerceptions.push_back(cellPerceptions);
    }
    return sps;
}

void ContainerUtils::toMsg(srg::sim::containers::SimPerceptions sp, ::capnp::MallocMessageBuilder& builder)
{
    SimPerceptionsMsg::Builder msg = builder.getRoot<SimPerceptionsMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(sp.receiverID->getRaw(), (unsigned int) sp.receiverID->getSize()));
    receiverID.setType(sp.receiverID->getType());

    ::capnp::List<::srg::sim::SimPerceptionsMsg::CellPerceptions>::Builder cellPerceptionsBuilder = msg.initCellPerceptions(sp.cellPerceptions.size());
    for (unsigned int i = 0; i < sp.cellPerceptions.size(); i++) {
        srg::sim::SimPerceptionsMsg::CellPerceptions::Builder cpBuilder = cellPerceptionsBuilder[i];
        cpBuilder.setX(sp.cellPerceptions[i].x);
        cpBuilder.setY(sp.cellPerceptions[i].y);

        ::capnp::List<::srg::sim::SimPerceptionsMsg::Perception>::Builder perceptionsBuilder =
                cpBuilder.initPerceptions(sp.cellPerceptions[i].perceptions.size());
        for (unsigned int j = 0; j < sp.cellPerceptions[i].perceptions.size(); j++) {
            srg::sim::SimPerceptionsMsg::Perception::Builder pBuilder = perceptionsBuilder[j];
            pBuilder.setY(sp.cellPerceptions[i].perceptions[j].y);
            pBuilder.setX(sp.cellPerceptions[i].perceptions[j].x);
            switch (sp.cellPerceptions[i].perceptions[j].type) {
            case srg::world::ObjectType::Robot:
                pBuilder.setType(srg::sim::SimPerceptionsMsg::Perception::Type::ROBOT);
                break;
            case srg::world::ObjectType::CupRed:
                pBuilder.setType(srg::sim::SimPerceptionsMsg::Perception::Type::CUPRED);
                break;
            case srg::world::ObjectType::CupBlue:
                pBuilder.setType(srg::sim::SimPerceptionsMsg::Perception::Type::CUPBLUE);
                break;
            case srg::world::ObjectType::CupYellow:
                pBuilder.setType(srg::sim::SimPerceptionsMsg::Perception::Type::CUPYELLOW);
                break;
            case srg::world::ObjectType::Door:
                pBuilder.setType(srg::sim::SimPerceptionsMsg::Perception::Type::DOOR);
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toMsg(): Unknown object type perceived: " << static_cast<int>(sp.cellPerceptions[i].perceptions[j].type)
                          << "!" << std::endl;
                break;
            }
            switch (sp.cellPerceptions[i].perceptions[j].state) {
            case srg::world::ObjectState::Open:
                pBuilder.setState(srg::sim::SimPerceptionsMsg::Perception::State::OPEN);
                break;
            case srg::world::ObjectState::Closed:
                pBuilder.setState(srg::sim::SimPerceptionsMsg::Perception::State::CLOSED);
                break;
            case srg::world::ObjectState::Carried:
                pBuilder.setState(srg::sim::SimPerceptionsMsg::Perception::State::CARRIED);
                break;
            case srg::world::ObjectState::Undefined:
                pBuilder.setState(srg::sim::SimPerceptionsMsg::Perception::State::UNDEFINED);
                break;
            default:
                std::cerr << "srgsim::ContainterUtils::toMsg(): Unknown object state perceived: "
                          << static_cast<int>(sp.cellPerceptions[i].perceptions[j].state) << "!" << std::endl;
                break;
            }
            capnzero::ID::Builder objectID = pBuilder.initObjectID();
            objectID.setType(sp.cellPerceptions[i].perceptions[j].objectID->getType());
            objectID.setValue(
                    ::capnp::Data::Reader(sp.cellPerceptions[i].perceptions[j].objectID->getRaw(), sp.cellPerceptions[i].perceptions[j].objectID->getSize()));
            capnzero::ID::Builder robotID = pBuilder.initRobotID();
            if (sp.cellPerceptions[i].perceptions[j].robotID.get()) {
                robotID.setType(sp.cellPerceptions[i].perceptions[j].robotID->getType());
                robotID.setValue(
                        ::capnp::Data::Reader(sp.cellPerceptions[i].perceptions[j].robotID->getRaw(), sp.cellPerceptions[i].perceptions[j].robotID->getSize()));
            }
        }
    }
}
} // namespace sim
} // namespace srg