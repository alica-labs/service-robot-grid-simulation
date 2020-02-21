#include "srg/sim/ContainerUtils.h"

#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>
#include <srg/sim/msgs/SimCommandMsg.capnp.h>

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
    case srg::sim::SimCommandMsg::Action::SPAWNROBOT:
        sc.action = containers::SimCommand::SPAWNROBOT;
        break;
    case srg::sim::SimCommandMsg::Action::SPAWNHUMAN:
        sc.action = containers::SimCommand::SPAWNHUMAN;
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
    case containers::SimCommand::SPAWNROBOT:
        msg.setAction(srg::sim::SimCommandMsg::Action::SPAWNROBOT);
        break;
    case containers::SimCommand::SPAWNHUMAN:
        msg.setAction(srg::sim::SimCommandMsg::Action::SPAWNHUMAN);
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
    srg::sim::SimPerceptionMsg::Reader reader = msg.getRoot<srg::sim::SimPerceptionMsg>();
    sps.receiverID = idManager->getIDFromBytes(
            reader.getReceiverID().getValue().asBytes().begin(), reader.getReceiverID().getValue().size(), reader.getReceiverID().getType());
    for (srg::sim::SimPerceptionMsg::CellPerception::Reader cellPerceptionMsg : reader.getCellPerceptions()) {
        srg::sim::containers::CellPerception cellPerception;
        cellPerception.x = cellPerceptionMsg.getX();
        cellPerception.y = cellPerceptionMsg.getY();

        for (srg::sim::SimPerceptionMsg::Object::Reader perceptionMsg : cellPerceptionMsg.getObjects()) {
            cellPerception.objects.push_back(ContainerUtils::createObject(perceptionMsg, idManager));
        }
        sps.cellPerceptions.push_back(cellPerception);
    }
    return sps;
}

std::shared_ptr<srg::world::Object> ContainerUtils::createObject(srg::sim::SimPerceptionMsg::Object::Reader& objectReader, essentials::IDManager* idManager)
{
    // ID
    essentials::IdentifierConstPtr id = idManager->getIDFromBytes(
            objectReader.getId().getValue().asBytes().begin(), objectReader.getId().getValue().size(), objectReader.getId().getType());
    // TYPE
    srg::world::ObjectType type;
    switch (objectReader.getType()) {
    case srg::sim::SimPerceptionMsg::Object::Type::ROBOT:
        type = srg::world::ObjectType::Robot;
        break;
    case srg::sim::SimPerceptionMsg::Object::Type::HUMAN:
        type = srg::world::ObjectType::Human;
        break;
    case srg::sim::SimPerceptionMsg::Object::Type::DOOR:
        type = srg::world::ObjectType::Door;
        break;
    case srg::sim::SimPerceptionMsg::Object::Type::CUPRED:
        type = srg::world::ObjectType::CupRed;
        break;
    case srg::sim::SimPerceptionMsg::Object::Type::CUPBLUE:
        type = srg::world::ObjectType::CupBlue;
        break;
    case srg::sim::SimPerceptionMsg::Object::Type::CUPYELLOW:
        type = srg::world::ObjectType::CupYellow;
        break;
    default:
        std::cerr << "[ContainterUtils] Unknown object type in capnp message found!" << std::endl;
        break;
    }

    // STATE
    srg::world::ObjectState state;
    switch (objectReader.getState()) {
    case srg::sim::SimPerceptionMsg::Object::State::OPEN:
        state = srg::world::ObjectState::Open;
        break;
    case srg::sim::SimPerceptionMsg::Object::State::CLOSED:
        state = srg::world::ObjectState::Closed;
        break;
    case srg::sim::SimPerceptionMsg::Object::State::UNDEFINED:
        state = srg::world::ObjectState::Undefined;
        break;
    default:
        std::cerr << "[ContainterUtils] Unknown object type in capnp message found!" << std::endl;
        break;
    }
    std::shared_ptr<srg::world::Object> object = std::make_shared<srg::world::Object>(id, type, state);
    for (srg::sim::SimPerceptionMsg::Object::Reader childObjectReader : objectReader.getObjects()) {
        object->addObject(ContainerUtils::createObject(childObjectReader, idManager));
    }
    return object;
}

void ContainerUtils::toMsg(srg::sim::containers::SimPerceptions sp, ::capnp::MallocMessageBuilder& builder)
{
    SimPerceptionMsg::Builder msg = builder.getRoot<SimPerceptionMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(sp.receiverID->getRaw(), (unsigned int) sp.receiverID->getSize()));
    receiverID.setType(sp.receiverID->getType());

    ::capnp::List<::srg::sim::SimPerceptionMsg::CellPerception>::Builder cellPerceptionsListBuilder = msg.initCellPerceptions(sp.cellPerceptions.size());
    for (unsigned int i = 0; i < sp.cellPerceptions.size(); i++) {
        srg::sim::SimPerceptionMsg::CellPerception::Builder cellPerceptionBuilder = cellPerceptionsListBuilder[i];
        cellPerceptionBuilder.setX(sp.cellPerceptions[i].x);
        cellPerceptionBuilder.setY(sp.cellPerceptions[i].y);
        ::capnp::List<::srg::sim::SimPerceptionMsg::Object>::Builder objectsListBuilder =
                cellPerceptionBuilder.initObjects(sp.cellPerceptions[i].objects.size());
        ContainerUtils::toObjectListMsg(sp.cellPerceptions[i].objects, objectsListBuilder);
    }
}

void ContainerUtils::toObjectListMsg(
        std::vector<std::shared_ptr<srg::world::Object>>& objects, ::capnp::List<::srg::sim::SimPerceptionMsg::Object>::Builder& objectsListBuilder)
{
    for (unsigned int j = 0; j < objects.size(); j++) {
        srg::sim::SimPerceptionMsg::Object::Builder objectBuilder = objectsListBuilder[j];
        ::capnp::List<::srg::sim::SimPerceptionMsg::Object>::Builder childObjectsListBuilder = objectBuilder.initObjects(objects[j]->getObjects().size());
        if (objects[j]->getObjects().size() > 0) {
            std::vector<std::shared_ptr<srg::world::Object>> childObject;
            for (auto& objectEntry : objects[j]->getObjects()) {
                childObject.push_back(objectEntry.second);
            }
            ContainerUtils::toObjectListMsg(childObject, childObjectsListBuilder);
        }

        switch (objects[j]->getType()) {
        case srg::world::ObjectType::Robot:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::ROBOT);
            break;
        case srg::world::ObjectType::Human:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::HUMAN);
            break;
        case srg::world::ObjectType::CupRed:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::CUPRED);
            break;
        case srg::world::ObjectType::CupBlue:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::CUPBLUE);
            break;
        case srg::world::ObjectType::CupYellow:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::CUPYELLOW);
            break;
        case srg::world::ObjectType::Door:
            objectBuilder.setType(srg::sim::SimPerceptionMsg::Object::Type::DOOR);
            break;
        default:
            std::cerr << "[ContainterUtils] Unknown object type perceived: " << objects[j]->getType() << std::endl;
            break;
        }
        switch (objects[j]->getState()) {
        case srg::world::ObjectState::Open:
            objectBuilder.setState(srg::sim::SimPerceptionMsg::Object::State::OPEN);
            break;
        case srg::world::ObjectState::Closed:
            objectBuilder.setState(srg::sim::SimPerceptionMsg::Object::State::CLOSED);
            break;
        case srg::world::ObjectState::Undefined:
            objectBuilder.setState(srg::sim::SimPerceptionMsg::Object::State::UNDEFINED);
            break;
        default:
            std::cerr << "[ContainterUtils] Unknown object state perceived: " << objects[j]->getState() << std::endl;
            break;
        }
        capnzero::ID::Builder objectID = objectBuilder.initId();
        objectID.setType(objects[j]->getID()->getType());
        objectID.setValue(::capnp::Data::Reader(objects[j]->getID()->getRaw(), objects[j]->getID()->getSize()));
    }
}
} // namespace sim
} // namespace srg