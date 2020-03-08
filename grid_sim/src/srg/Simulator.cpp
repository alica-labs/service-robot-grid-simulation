#include "srg/Simulator.h"

#include "srg/sim/Sensor.h"
#include "srg/sim/SimulatedAgent.h"
#include "srg/sim/commands/CommandHandler.h"
#include "srg/sim/commands/ManipulationHandler.h"
#include "srg/sim/commands/MoveCommandHandler.h"
#include "srg/sim/commands/SpawnCommandHandler.h"
#include "srg/sim/communication/Communication.h"

#include <srg/GUI.h>
#include <srg/World.h>
#include <srg/world/Agent.h>

#include <SystemConfig.h>
#include <essentials/IDManager.h>

#include <cassert>
#include <iostream>
#include <signal.h>
#include <string>
#include <thread>

//#define SIM_DEBUG

namespace srg
{
bool Simulator::running = false;

Simulator::Simulator(bool headless)
        : headless(headless)
        , idManager(new essentials::IDManager())
{
    this->world = new World(this->idManager);
    this->placeObjectsFromConf();
    this->communicationHandlers.push_back(new sim::commands::MoveCommandHandler(this));
    this->communicationHandlers.push_back(new sim::commands::ManipulationHandler(this));
    this->communicationHandlers.push_back(new sim::commands::SpawnCommandHandler(this));
    this->communication = new sim::communication::Communication(this->idManager, this);
}

void Simulator::placeObjectsFromConf()
{
    this->sc = essentials::SystemConfig::getInstance();
    std::shared_ptr<std::vector<std::string>> objectSections = (*sc)["Objects"]->getSections("Objects", NULL);
    for (std::string objectSection : *objectSections) {
        int32_t intObjectID = std::stoi(objectSection);
        essentials::IdentifierConstPtr id = essentials::IdentifierConstPtr(this->idManager->getID<int32_t>(intObjectID));
        std::string stringObjectType = (*sc)["Objects"]->get<std::string>("Objects", objectSection.c_str(), "type", NULL);
        world::ObjectType type;
        if (stringObjectType.compare("cup_blue") == 0) {
            type = world::ObjectType::CupBlue;
        } else if (stringObjectType.compare("cup_red") == 0) {
            type = world::ObjectType::CupRed;
        } else if (stringObjectType.compare("cup_yellow") == 0) {
            type = world::ObjectType::CupYellow;
        } else if (stringObjectType.compare("door") == 0) {
            type = world::ObjectType::Door;
        } else {
            continue;
        }

        std::shared_ptr<world::Object> object;
        if (type == world::ObjectType::Door) {
            if ((*sc)["Objects"]->get<bool>("Objects", objectSection.c_str(), "open", NULL)) {
                object = this->world->createOrUpdateObject(std::make_shared<world::Object>(id, type, world::ObjectState::Open));
            } else {
                object = this->world->createOrUpdateObject(std::make_shared<world::Object>(id, type, world::ObjectState::Closed));
            }
        } else {
            object = this->world->createOrUpdateObject(std::make_shared<world::Object>(id, type));
        }

        if (object->getParentContainer()) {
            // object is already placed, maybe it was created already...
            continue;
        }

        world::Coordinate coord((*sc)["Objects"]->get<uint32_t>("Objects", objectSection.c_str(), "x", NULL),
                (*sc)["Objects"]->get<uint32_t>("Objects", objectSection.c_str(), "y", NULL));
        if (!this->world->placeObject(object, coord)) {
            std::cout << "[Simulator] Placement of " << type << " to " << coord << " not allowed!" << std::endl;
        }
    }
}

Simulator::~Simulator()
{

    delete this->communication;
    for (auto& handler : this->communicationHandlers) {
        delete handler;
    }
    delete this->idManager;
    delete this->world;
    delete this->gui;
}

srg::World* Simulator::getWorld()
{
    return this->world;
}

void Simulator::addSimulatedAgent(std::shared_ptr<world::Agent> agent)
{
    if (!agent)
        return;

    if (std::find_if(simulatedAgents.begin(), simulatedAgents.end(), [agent](sim::SimulatedAgent* a) { return a->getID() == agent->getID(); }) !=
            simulatedAgents.end())
        return;

    std::cout << "[Simulator] Adding " << *agent << std::endl;
    this->simulatedAgents.push_back(new sim::SimulatedAgent(agent));
}

sim::SimulatedAgent* Simulator::getAgent(essentials::IdentifierConstPtr id)
{
    for (sim::SimulatedAgent* robot : simulatedAgents) {
        if (robot->getID() == id) {
            return robot;
        }
    }
    return nullptr;
}

void Simulator::addMarker(viz::Marker marker)
{
    this->gui->addMarker(marker);
}

void Simulator::start()
{
    if (!Simulator::running) {
        Simulator::running = true;
        this->mainThread = new std::thread(&Simulator::run, this);
    }
}

void Simulator::run()
{
    if (!this->headless) {
        this->gui = new GUI("Grid Simulator GUI");
    }
    while (Simulator::running) {
#ifdef SIM_DEBUG
        std::cout << "[Simulator] Iteration started..." << std::endl;
        std::cout << "[Simulator] Updating GUI..." << std::endl;
#endif
        auto start = std::chrono::system_clock::now();

        // Update GUI
        if (!this->headless) {
            this->gui->draw(this->world);
        }

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Handle commands..." << std::endl;
#endif
        // Handle Commands
        {
            std::lock_guard<std::recursive_mutex> guard(commandMutex);
            while (this->commandQueue.size() > 0) {
                srg::sim::containers::SimCommand sc = this->commandQueue.front();
                this->commandQueue.pop();
                for (sim::commands::CommandHandler* handler : this->communicationHandlers) {
                    if (handler->handle(sc)) {
                        break;
                    }
                }
            }
        }

        // Displace some object (almost) randomly
        if (rand() % 200 < 2) { // 1% chance of displacing an object
            this->world->displaceObject();
        }

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Create and send perceptions..." << std::endl;
#endif
        // Produce and send perceptions for each robot
        assert(this->simulatedAgents.size() < 4);
        for (auto& simulatedAgent : this->simulatedAgents) {
            this->communication->sendSimPerceptions(simulatedAgent->createSimPerceptions(this));
        }

        // Sleep in order to keep the cpu effort low
        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::milliseconds millisecondsPassed = std::chrono::duration_cast<std::chrono::milliseconds>(timePassed);

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Iteration took " << millisecondsPassed.count() << " ms. Sleep " << 20 - millisecondsPassed.count()
                  << "ms to keep frequency..." << std::endl;
#endif
        if (millisecondsPassed.count() < 20) { // Simulator frequency is 50 times per seconds!
            std::this_thread::sleep_for(std::chrono::milliseconds(20 - millisecondsPassed.count()));
        }
#ifdef SIM_DEBUG
        std::cout << "[Simulator] ...iteration end!\n------------------------------" << std::endl;
#endif
    }
}

void Simulator::processSimCommand(srg::sim::containers::SimCommand sc)
{
    std::lock_guard<std::recursive_mutex> guard(commandMutex);
    this->commandQueue.push(sc);
}

bool Simulator::isRunning()
{
    return running;
}

/**
 * This is for handling [Ctrl] + [c]
 * @param sig
 */
void Simulator::simSigintHandler(int sig)
{
    std::cout << "[Simulator] Caught SIGINT! Terminating ..." << std::endl;
    running = false;
}

} // namespace srg

int main(int argc, char* argv[])
{
    bool headless = false;
    if (argc > 1) {
        if (std::string("--headless") == argv[1]) {
            headless = true;
        }
    }

    srg::Simulator* simulator = new srg::Simulator(headless);

    signal(SIGINT, srg::Simulator::simSigintHandler);

    simulator->start();

    while (simulator->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete simulator;

    return 0;
}
