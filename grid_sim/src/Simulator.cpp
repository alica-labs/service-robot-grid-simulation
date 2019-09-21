#include "srgsim/Simulator.h"

#include "srgsim/commands/CommandHandler.h"
#include "srgsim/commands/DoorCommandHandler.h"
#include "srgsim/commands/MoveCommandHandler.h"
#include "srgsim/commands/PickUpCommandHandler.h"
#include "srgsim/commands/PutDownCommandHandler.h"
#include "srgsim/commands/SpawnCommandHandler.h"

#include "srgsim/world/Cell.h"
#include "srgsim/GUI.h"
#include "srgsim/world/World.h"
#include "srgsim/world/ServiceRobot.h"
#include "srgsim/communication/Communication.h"

#include <essentials/IDManager.h>

#include <iostream>
#include <signal.h>
#include <string>
#include <thread>

//#define SIM_DEBUG

namespace srgsim
{
bool Simulator::running = false;

Simulator::Simulator(bool headless)
        : headless(headless)
        , idManager(new essentials::IDManager())
{
    this->world = new World();
    this->communicationHandlers.push_back(new commands::MoveCommandHandler(world));
    this->communicationHandlers.push_back(new commands::DoorCommandHandler(world));
    this->communicationHandlers.push_back(new commands::PickUpCommandHandler(world));
    this->communicationHandlers.push_back(new commands::PutDownCommandHandler(world));
    this->communicationHandlers.push_back(new commands::SpawnCommandHandler(world));
    this->communication = new communication::Communication(this->idManager, this);
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
        this->gui = new GUI();
    }
    while (Simulator::running) {
#ifdef SIM_DEBUG
        std::cout << "[Simulator] Iteration started..." << std::endl;
        std::cout << "[Simulator] Updating GUI..." << std::endl;
#endif
        auto start = std::chrono::system_clock::now();

        // 1. Update GUI
        if (!this->headless) {
            this->gui->draw(this->world);
        }

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Handle commands..." << std::endl;
#endif
        // 2. Handle Commands
        while(this->commandQueue.size() > 0) {
            SimCommand sc = this->commandQueue.front();
            this->commandQueue.pop();
            for (commands::CommandHandler* handler : this->communicationHandlers) {
                if (handler->handle(sc)) {
                    break;
                }
            }
        }

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Create and send perceptions..." << std::endl;
#endif
        // 3. Produce and send perceptions for each robot
        std::vector<SimPerceptions> perceptionsMsgs = this->world->createSimPerceptions();
        for (auto& simPerceptionsMsg : perceptionsMsgs) {
            this->communication->sendSimPerceptions(simPerceptionsMsg);
        }

        // 4. Sleep in order to keep the cpu effort low
        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::milliseconds millisecondsPassed = std::chrono::duration_cast<std::chrono::milliseconds>(timePassed);

#ifdef SIM_DEBUG
        std::cout << "[Simulator] Sleep " <<  millisecondsPassed.count() << "ms to keep frequency..." << std::endl;
#endif
        if (millisecondsPassed.count() < 33) { // Simulator frequency is 30 times per seconds!
            std::this_thread::sleep_for(std::chrono::milliseconds(33-millisecondsPassed.count()));
        }
#ifdef SIM_DEBUG
        std::cout << "[Simulator] ...iteration end!\n------------------------------" << std::endl;
#endif
    }
}

void Simulator::processSimCommand(SimCommand sc) {
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
    std::cout << "Simulator: Caught SIGINT! Terminating ..." << std::endl;
    running = false;
}

} // namespace srgsim

int main(int argc, char* argv[])
{
    bool headless = false;
    if (argc > 1) {
        if (std::string("--headless") == argv[1]) {
            headless = true;
        }
    }

    srgsim::Simulator* simulator = new srgsim::Simulator(headless);

    signal(SIGINT, srgsim::Simulator::simSigintHandler);

    simulator->start();

    while (simulator->isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete simulator;

    return 0;
}
