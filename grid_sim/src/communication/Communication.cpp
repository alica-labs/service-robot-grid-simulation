#include "srgsim/communication/Communication.h"

#include "srgsim/communication/SpawnCommandHandler.h"
#include "srgsim/communication/DoorCommandHandler.h"
#include "srgsim/communication/MoveCommandHandler.h"
#include "srgsim/communication/PickUpCommandHandler.h"
#include "srgsim/communication/PutDownCommandHandler.h"

#include "srgsim/Command.capnp.h"
#include "srgsim/Simulator.h"

#include <capnp/common.h>
#include <capnp/message.h>
#include <capnzero/Subscriber.h>

#include <essentials/IDManager.h>

#include <vector>

namespace srgsim
{
namespace communication
{
const std::string Communication::commandTopic = "/srgsim/cmd";

Communication::Communication(Simulator* simulator)
        : simulator(simulator)
{
    this->communicationHandlers.push_back(new communication::MoveCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::DoorCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::PickUpCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::PutDownCommandHandler(simulator));
    this->communicationHandlers.push_back(new communication::SpawnCommandHandler(simulator));

    this->ctx = zmq_ctx_new();
    this->url = "224.0.0.2:5555";
    this->commandSubscriber = new capnzero::Subscriber(this->ctx, this->commandTopic);
    this->commandSubscriber->connect(capnzero::CommType::UDP, this->url);
    this->commandSubscriber->subscribe(&Communication::commandCallback, &(*this));
}

Communication::~Communication()
{
    for(auto& handler : this->communicationHandlers) {
        delete handler;
    }
    delete this->commandSubscriber;
    zmq_ctx_term(this->ctx);
}

void Communication::commandCallback(::capnp::FlatArrayMessageReader& msg)
{
    srgsim::Command::Reader reader = msg.getRoot<srgsim::Command>();
    Command::Action action = reader.getAction();

    for (CommandHandler* handler: this->communicationHandlers) {
        if (handler->handle(action, msg)) {
            break;
        }
    }

}
} // namespace communication
} // namespace srgsim