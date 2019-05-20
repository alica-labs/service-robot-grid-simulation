#include "srgsim/Simulator.h"

#include "srgsim/Cell.h"
#include "srgsim/communication/Communication.h"
#include "srgsim/GUI.h"
#include "srgsim/Object.h"
#include "srgsim/SRGIDManager.h"
#include "srgsim/World.h"

#include <iostream>
#include <limits.h>
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
        , idManager(new SRGIDManager())
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
    const essentials::ID* robotID = this->idManager->generateID();
    this->spawnRobot(robotID);
    if (!this->headless) {
        this->gui = new GUI();
    }
    while (Simulator::running) {
#ifdef SIM_DEBUG
        auto start = std::chrono::system_clock::now();
        std::cout << "[Simulator] Iteration started..." << std::endl;
#endif
        if (!this->headless) {
            this->gui->draw(this->world);
        }
#ifdef SIM_DEBUG
        auto timePassed = std::chrono::system_clock::now() - start;
        std::chrono::microseconds microsecondsPassed = std::chrono::duration_cast<std::chrono::microseconds>(timePassed);
        std::cout << "[Simulator] ... took " << microsecondsPassed.count() << " microsecs" << std::endl;
#endif

        // TODO remove later, just for debug
        std::cout << "Test: Simulator: moving Robot left" << std::endl;
        unsigned int microseconds = 1000000;
        usleep(microseconds);
        this->moveObject(robotID, Direction::Left);
    }
}

std::string Simulator::getSelfPath()
{
    char buff[PATH_MAX];
    ssize_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1) {
        buff[len] = '\0';
        std::string exePath = std::string(buff);

        return exePath.substr(0, exePath.find_last_of("/"));
    }
    /* handle error condition */
    return "Simulator: Path not Found!";
}

bool Simulator::isRunning()
{
    return running;
}

SRGIDManager* Simulator::getIdManager() const
{
    return idManager;
}

/////////////////////////// Interaction /////////////////////////////////////

void Simulator::spawnRobot(const essentials::ID* id)
{
    // create robot
    Object* object = new Object(Type::Robot, id);
    this->objects.emplace(object->getID(), object);

    // search for cell with valid spawn coordinates
    srand(time(NULL));
    Cell* cell = nullptr;
    while (!cell || cell->type != Type::Floor) {
        cell = world->getCell(Coordinate(rand() % world->getSizeX(), rand() % world->getSizeX()));
    }

    // place robot
    world->placeObject(object, cell->coordinate);
}

void Simulator::moveObject(const essentials::ID* id, Direction direction)
{
    auto iter = objects.find(essentials::IDConstPtr(id));
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
    return !(cell->type == Type::Door || cell->type == Type::Default || cell->type == Type::Wall);
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
