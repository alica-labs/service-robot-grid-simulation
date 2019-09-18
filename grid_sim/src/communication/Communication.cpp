#include "srgsim/communication/Communication.h"

#include "srgsim/communication/DoorCommandHandler.h"
#include "srgsim/communication/MoveCommandHandler.h"
#include "srgsim/communication/PickUpCommandHandler.h"
#include "srgsim/communication/PutDownCommandHandler.h"
#include "srgsim/communication/SpawnCommandHandler.h"

#include "srgsim/Simulator.h"

#include <srgsim/containers/ContainerUtils.h>

#include <capnzero/Subscriber.h>

#include <essentials/IDManager.h>

#include <vector>

namespace srgsim
{
namespace communication
{

Communication::Communication(Simulator* simulator)
        : simulator(simulator)
{
    this->communicationHandlers.push_back(new communication::MoveCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::DoorCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::PickUpCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::PutDownCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::SpawnCommandHandler(simulator));

    this->sc = essentials::SystemConfig::getInstance();
    this->ctx = zmq_ctx_new();
    this->address = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.address", NULL);
    this->simCommandTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.cmdTopic", NULL);

    this->simCommandSub = new capnzero::Subscriber(this->ctx, capnzero::Protocol::UDP);
    this->simCommandSub->setTopic(this->simCommandTopic);
    this->simCommandSub->addAddress(this->address);
    this->simCommandSub->subscribe(&Communication::SimCommandCallback, &(*this));

    this->simPerceptionsTopic = (*sc)["SRGSim"]->get<std::string>("SRGSim.Communication.perceptionsTopic", NULL);
    this->simPerceptionsPub = new capnzero::Publisher(this->ctx, capnzero::Protocol::UDP);
    this->simPerceptionsPub->setDefaultTopic(simPerceptionsTopic);
    this->simPerceptionsPub->addAddress(this->address);
}

Communication::~Communication()
{
    for (auto& handler : this->communicationHandlers) {
        delete handler;
    }
    delete this->simCommandSub;
    zmq_ctx_term(this->ctx);
}

void Communication::SimCommandCallback(::capnp::FlatArrayMessageReader& msg)
{
    for (CommandHandler* handler : this->communicationHandlers) {
        if (handler->handle(ContainerUtils::toSimCommand(msg, this->simulator->getIdManager()))) {
            break;
        }
    }
}

void Communication::sendSimPerceptions(srgsim::SimPerceptions sp) {
    ::capnp::MallocMessageBuilder msgBuilder;
    ContainerUtils::toMsg(sp, msgBuilder);
    this->simPerceptionsPub->send(msgBuilder);
}
} // namespace communication
} // namespace srgsim
