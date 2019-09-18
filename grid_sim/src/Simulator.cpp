#include "srgsim/Simulator.h"

#include "srgsim/Cell.h"
#include "srgsim/GUI.h"
#include "srgsim/Object.h"
#include "srgsim/World.h"
#include "srgsim/ServiceRobot.h"
#include "srgsim/communication/Communication.h"

#include <essentials/IDManager.h>

#include <iostream>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>

//#define SIM_DEBUG

namespace srgsim
{
bool Simulator::running = false;

Simulator::Simulator(bool headless)
        : headless(headless)
        , idManager(new essentials::IDManager())
{
    // this->initTestWorld();
    this->communication = new communication::Communication(this);
    this->initWorld();
}

Simulator::~Simulator()
{
    for (auto& object : objects) {
        delete object.second;
    }
    delete this->communication;
    delete this->idManager;
    delete this->world;
    delete this->gui;
}

void Simulator::initWorld()
{
    this->world = new World();
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
    // TODO remove later, just for debug
    const essentials::Identifier* robotID = this->idManager->generateID();
    this->spawnRobot(robotID);

    if (!this->headless) {
        this->gui = new GUI();
    }
    while (Simulator::running) {
#ifdef SIM_DEBUG
        std::cout << "[Simulator] Iteration started..." << std::endl;
#endif
        auto start = std::chrono::system_clock::now();

        // 1. Update GUI
        if (!this->headless) {
            this->gui->draw(this->world);
        }

        // 2. Produce and send perceptions for each robot
        for (auto& robotEntry : this->robots) {
            this->communication->sendSimPerceptions(robotEntry.second->createSimPerceptions(this->world));
        }


        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::milliseconds millisecondsPassed = std::chrono::duration_cast<std::chrono::milliseconds>(timePassed);

        if (millisecondsPassed.count() < 33) {
            std::this_thread::sleep_for(std::chrono::milliseconds(33-millisecondsPassed.count()));
        }
#ifdef SIM_DEBUG
        std::cout << "[Simulator] ... took " << millisecondsPassed.count() << " milliseconds" << std::endl;
#endif

        // TODO remove later, just for debug
        std::cout << "Test: Simulator: Sleep and moving Robot left" << std::endl;
        unsigned int microseconds = 1000000;
        usleep(microseconds);
        this->moveObject(robotID, Direction::Left);
    }
}

bool Simulator::isRunning()
{
    return running;
}

essentials::IDManager* Simulator::getIdManager() const
{
    return idManager;
}

/////////////////////////// Interaction /////////////////////////////////////

bool Simulator::spawnRobot(const essentials::Identifier* id)
{
    // create robot
    Object* object = this->addObject(id, Type::Robot);
    if (object->getCell()) {
        // robot is already placed, maybe it was spawned already...
        return false;
    }

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    Cell* cell = nullptr;
    while (!cell || isPlacementAllowed(cell, Type::Robot)) {
        cell = world->getCell(Coordinate(rand() % world->getSizeX(), rand() % world->getSizeX()));
    }

    // place robot
    if (world->placeObject(object, cell->coordinate)) {
        this->robots.emplace(object->getID(), static_cast<ServiceRobot*>(object));
        return true;
    } else {
        return false;
    }
    std::cout << "placing robot . . .\n";
    world->placeObject(object, cell->coordinate);
}

Object* Simulator::addObject(const essentials::Identifier* id, Type type)
{
    auto objectEntry = this->objects.find(essentials::IdentifierConstPtr(id));
    if (objectEntry == this->objects.end()) {
        Object* object;
        if (type == Type::Robot) {
            object = new ServiceRobot(id);
        } else {
            object = new Object(type, id);
        }
        this->objects.emplace(object->getID(), object);
        return object;
    } else {
        if (objectEntry->second->getType() != type) {
            std::cerr << "Simulator::addObject(): Object ID " << id << " is already known as type (" << objectEntry->second->getType() << "), but requested type is (" << type << ")!" << std::endl;
            return nullptr;
        } else {
            return objectEntry->second;
        }
    }
}

void Simulator::moveObject(const essentials::Identifier* id, Direction direction)
{
    auto iter = objects.find(essentials::IdentifierConstPtr(id));
    if (iter == objects.end()) {
        std::cout << "Simulator::moveObject: unknown object ID " << *id << " requested!" << std::endl;
        return;
    }
    Cell* goalCell = getNeighbourCell(direction, iter->second);
    if (!goalCell) {
        return;
    }
    if (!isPlacementAllowed(goalCell, iter->second->getType())) {
        return;
    }
    iter->second->getCell()->removeObject(iter->second);
    goalCell->addObject(iter->second);
}

Cell* Simulator::getNeighbourCell(const Direction& direction, Object* object)
{
    switch (direction) {
    case Left:
        return object->getCell()->left;
    case Up:
        return object->getCell()->up;
    case Right:
        return object->getCell()->right;
    case Down:
        return object->getCell()->down;
    default:
        std::cout << "Simulator: Unknown Direction: " << direction << "!" << std::endl;
        return nullptr;
    }
}

bool Simulator::isPlacementAllowed(Cell* cell, Type objectType)
{
    switch (objectType) {
    case Type::Robot:
        return cell->type == Type::Floor;
    default:
        return !(cell->type == Type::Door || cell->type == Type::Wall);
    }
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
