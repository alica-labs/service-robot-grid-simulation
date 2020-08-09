#include "srg/sim/ContainerUtils.h"

#include <essentials/IDManager.h>
#include <essentials/WildcardID.h>
#include <srg/sim/msgs/SimCommandMsg.capnp.h>

namespace srg
{
namespace sim
{
containers::SimCommand ContainerUtils::toSimCommand(::capnp::FlatArrayMessageReader& msg, essentials::IDManager& idManager)
{
    containers::SimCommand sc;
    srg::sim::SimCommandMsg::Reader reader = msg.getRoot<srg::sim::SimCommandMsg>();
    sc.senderID = idManager.getIDFromBytes(
            reader.getSenderID().getValue().asBytes().begin(), reader.getSenderID().getValue().size(), reader.getSenderID().getType());
    sc.objectID = idManager.getIDFromBytes(
            reader.getObjectID().getValue().asBytes().begin(), reader.getObjectID().getValue().size(), reader.getObjectID().getType());
    sc.timestamp = std::chrono::nanoseconds (reader.getTimestamp());

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

    msg.setTimestamp(sc.timestamp.count());

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

containers::Perceptions ContainerUtils::toPerceptions(::capnp::FlatArrayMessageReader& msg, essentials::IDManager& idManager)
{
    srg::sim::PerceptionMsg::Reader reader = msg.getRoot<srg::sim::PerceptionMsg>();
    return ContainerUtils::createPerceptions(reader, idManager);
}

containers::Perceptions ContainerUtils::createPerceptions(srg::sim::PerceptionMsg::Reader perceptionsReader, essentials::IDManager& idManager)
{
    containers::Perceptions ps;

    ps.receiverID = idManager.getIDFromBytes(perceptionsReader.getReceiverID().getValue().asBytes().begin(),
            perceptionsReader.getReceiverID().getValue().size(), perceptionsReader.getReceiverID().getType());
    ps.timestamp = std::chrono::nanoseconds(perceptionsReader.getTimestamp());
    for (srg::sim::PerceptionMsg::CellPerception::Reader cellPerceptionMsg : perceptionsReader.getCellPerceptions()) {
        srg::sim::containers::CellPerception cellPerception;
        cellPerception.x = cellPerceptionMsg.getX();
        cellPerception.y = cellPerceptionMsg.getY();

        for (srg::sim::PerceptionMsg::Object::Reader perceptionMsg : cellPerceptionMsg.getObjects()) {
            cellPerception.objects.push_back(ContainerUtils::createObject(perceptionMsg, idManager));
        }
        cellPerception.time = cellPerceptionMsg.getTime();
        ps.cellPerceptions.push_back(cellPerception);
    }
    return ps;
}

void ContainerUtils::toMsg(containers::Perceptions perceptions, ::srg::sim::PerceptionMsg::Builder& builder)
{
    capnzero::ID::Builder receiverID = builder.initReceiverID();
    receiverID.setValue(kj::arrayPtr(perceptions.receiverID->getRaw(), (unsigned int) perceptions.receiverID->getSize()));
    receiverID.setType(perceptions.receiverID->getType());

    builder.setTimestamp(perceptions.timestamp.count());

    ::capnp::List<::srg::sim::PerceptionMsg::CellPerception>::Builder cellPerceptionsListBuilder =
            builder.initCellPerceptions(perceptions.cellPerceptions.size());
    for (unsigned int i = 0; i < perceptions.cellPerceptions.size(); i++) {
        ::srg::sim::PerceptionMsg::CellPerception::Builder cellPerceptionBuilder = cellPerceptionsListBuilder[i];
        cellPerceptionBuilder.setTime(perceptions.cellPerceptions[i].time);
        cellPerceptionBuilder.setX(perceptions.cellPerceptions[i].x);
        cellPerceptionBuilder.setY(perceptions.cellPerceptions[i].y);
        ::capnp::List<::srg::sim::PerceptionMsg::Object>::Builder objectListBuilder = cellPerceptionBuilder.initObjects(perceptions.cellPerceptions[i].objects.size());
        srg::sim::ContainerUtils::toObjectListMsg(perceptions.cellPerceptions[i].objects,objectListBuilder);
    }
}

std::shared_ptr<srg::world::Object> ContainerUtils::createObject(srg::sim::PerceptionMsg::Object::Reader& objectReader, essentials::IDManager& idManager)
{
    // ID
    essentials::IdentifierConstPtr id = idManager.getIDFromBytes(
            objectReader.getId().getValue().asBytes().begin(), objectReader.getId().getValue().size(), objectReader.getId().getType());
    // TYPE
    srg::world::ObjectType type;
    switch (objectReader.getType()) {
    case srg::sim::PerceptionMsg::Object::Type::ROBOT:
        type = srg::world::ObjectType::Robot;
        break;
    case srg::sim::PerceptionMsg::Object::Type::HUMAN:
        type = srg::world::ObjectType::Human;
        break;
    case srg::sim::PerceptionMsg::Object::Type::DOOR:
        type = srg::world::ObjectType::Door;
        break;
    case srg::sim::PerceptionMsg::Object::Type::CUPRED:
        type = srg::world::ObjectType::CupRed;
        break;
    case srg::sim::PerceptionMsg::Object::Type::CUPBLUE:
        type = srg::world::ObjectType::CupBlue;
        break;
    case srg::sim::PerceptionMsg::Object::Type::CUPYELLOW:
        type = srg::world::ObjectType::CupYellow;
        break;
    default:
        std::cerr << "[ContainterUtils] Unknown object type in capnp message found!" << std::endl;
        break;
    }

    // STATE
    srg::world::ObjectState state;
    switch (objectReader.getState()) {
    case srg::sim::PerceptionMsg::Object::State::OPEN:
        state = srg::world::ObjectState::Open;
        break;
    case srg::sim::PerceptionMsg::Object::State::CLOSED:
        state = srg::world::ObjectState::Closed;
        break;
    case srg::sim::PerceptionMsg::Object::State::UNDEFINED:
        state = srg::world::ObjectState::Undefined;
        break;
    default:
        std::cerr << "[ContainterUtils] Unknown object type in capnp message found!" << std::endl;
        break;
    }
    std::shared_ptr<srg::world::Object> object = std::make_shared<srg::world::Object>(id, type, state);
    for (srg::sim::PerceptionMsg::Object::Reader childObjectReader : objectReader.getObjects()) {
        object->addObject(ContainerUtils::createObject(childObjectReader, idManager));
    }
    return object;
}

void ContainerUtils::toMsg(srg::sim::containers::Perceptions sp, ::capnp::MallocMessageBuilder& builder)
{
    PerceptionMsg::Builder msg = builder.getRoot<PerceptionMsg>();

    capnzero::ID::Builder receiverID = msg.initReceiverID();
    receiverID.setValue(kj::arrayPtr(sp.receiverID->getRaw(), (unsigned int) sp.receiverID->getSize()));
    receiverID.setType(sp.receiverID->getType());

    msg.setTimestamp(sp.timestamp.count());

    ::capnp::List<::srg::sim::PerceptionMsg::CellPerception>::Builder cellPerceptionsListBuilder = msg.initCellPerceptions(sp.cellPerceptions.size());
    for (unsigned int i = 0; i < sp.cellPerceptions.size(); i++) {
        srg::sim::PerceptionMsg::CellPerception::Builder cellPerceptionBuilder = cellPerceptionsListBuilder[i];
        cellPerceptionBuilder.setX(sp.cellPerceptions[i].x);
        cellPerceptionBuilder.setY(sp.cellPerceptions[i].y);
        ::capnp::List<::srg::sim::PerceptionMsg::Object>::Builder objectsListBuilder = cellPerceptionBuilder.initObjects(sp.cellPerceptions[i].objects.size());
        ContainerUtils::toObjectListMsg(sp.cellPerceptions[i].objects, objectsListBuilder);
        cellPerceptionBuilder.setTime(sp.cellPerceptions[i].time);
    }
}

void ContainerUtils::toObjectListMsg(
        std::vector<std::shared_ptr<srg::world::Object>>& objects, ::capnp::List<::srg::sim::PerceptionMsg::Object>::Builder& objectsListBuilder)
{
    for (unsigned int j = 0; j < objects.size(); j++) {
        srg::sim::PerceptionMsg::Object::Builder objectBuilder = objectsListBuilder[j];
        ::capnp::List<::srg::sim::PerceptionMsg::Object>::Builder childObjectsListBuilder = objectBuilder.initObjects(objects[j]->getObjects().size());
        if (objects[j]->getObjects().size() > 0) {
            std::vector<std::shared_ptr<srg::world::Object>> childObject;
            for (auto& objectEntry : objects[j]->getObjects()) {
                childObject.push_back(objectEntry.second);
            }
            ContainerUtils::toObjectListMsg(childObject, childObjectsListBuilder);
        }

        switch (objects[j]->getType()) {
        case srg::world::ObjectType::Robot:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::ROBOT);
            break;
        case srg::world::ObjectType::Human:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::HUMAN);
            break;
        case srg::world::ObjectType::CupRed:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::CUPRED);
            break;
        case srg::world::ObjectType::CupBlue:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::CUPBLUE);
            break;
        case srg::world::ObjectType::CupYellow:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::CUPYELLOW);
            break;
        case srg::world::ObjectType::Door:
            objectBuilder.setType(srg::sim::PerceptionMsg::Object::Type::DOOR);
            break;
        default:
            std::cerr << "[ContainterUtils] Unknown object type perceived: " << objects[j]->getType() << std::endl;
            break;
        }
        switch (objects[j]->getState()) {
        case srg::world::ObjectState::Open:
            objectBuilder.setState(srg::sim::PerceptionMsg::Object::State::OPEN);
            break;
        case srg::world::ObjectState::Closed:
            objectBuilder.setState(srg::sim::PerceptionMsg::Object::State::CLOSED);
            break;
        case srg::world::ObjectState::Undefined:
            objectBuilder.setState(srg::sim::PerceptionMsg::Object::State::UNDEFINED);
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