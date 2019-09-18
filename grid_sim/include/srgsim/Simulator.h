#pragma once

#include "containers/SimCommand.h"

#include "SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

#include <queue>

namespace std
{
class thread;
}

namespace essentials {
    class IDManager;
}

namespace srgsim
{

namespace communication
{
class Communication;
}

class Object;
class World;
class GUI;
class Cell;
class Simulator
{
public:
    Simulator(bool headless = false);
    ~Simulator();
    void start();
    void run();
    static bool isRunning();

    bool spawnRobot(const essentials::Identifier* id);
    void moveObject(const essentials::Identifier* id, Direction direction);
    essentials::IDManager* getIdManager() const;

    static void simSigintHandler(int sig);

private:
    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    communication::Communication* communication;

    essentials::IDManager* idManager;
    std::thread* mainThread;
    std::queue<SimCommand> commandQueue;
};
} // namespace srgsim
