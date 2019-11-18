#pragma once

#include "srg/sim/SimulatedRobot.h"
#include "srg/sim/containers/SimCommand.h"

#include <essentials/IdentifierConstPtr.h>

#include <mutex>
#include <queue>
#include <srg/viz/Marker.h>

namespace std
{
class thread;
}

namespace essentials
{
class IDManager;
class SystemConfig;
} // namespace essentials

namespace srg
{

namespace sim
{
namespace communication
{
class Communication;
}
namespace commands
{
class CommandHandler;
}
} // namespace sim

namespace world
{
class Object;
class Cell;
} // namespace world

class World;
class GUI;
class Simulator
{
public:
    Simulator(bool headless = false);
    ~Simulator();
    void start();
    void run();
    void addMarker(viz::Marker marker);
    srg::World* getWorld();
    void addRobot(world::ServiceRobot* serviceRobot);
    sim::SimulatedRobot* getRobot(essentials::IdentifierConstPtr id);
    static bool isRunning();
    static void simSigintHandler(int sig);
    void processSimCommand(sim::containers::SimCommand sc);

private:
    void placeObjectsFromConf();

    essentials::SystemConfig* sc;
    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    sim::communication::Communication* communication;
    std::vector<sim::SimulatedRobot*> simulatedRobots;

    essentials::IDManager* idManager;
    std::thread* mainThread;

    mutable std::recursive_mutex commandMutex;
    std::queue<sim::containers::SimCommand> commandQueue;
    std::vector<sim::commands::CommandHandler*> communicationHandlers;
};
} // namespace srg
