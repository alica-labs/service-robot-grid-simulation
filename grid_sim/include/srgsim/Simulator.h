#pragma once

#include "containers/SimCommand.h"

#include "SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

#include <queue>

namespace std
{
class thread;
}

namespace essentials
{
class IDManager;
}

namespace srgsim
{

namespace communication
{
class Communication;
}

namespace commands
{
class CommandHandler;
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
    static void simSigintHandler(int sig);
    void processSimCommand(SimCommand sc);

private:
    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    communication::Communication* communication;

    essentials::IDManager* idManager;
    std::thread* mainThread;
    std::queue<SimCommand> commandQueue;
    std::vector<commands::CommandHandler*> communicationHandlers;
};
} // namespace srgsim
