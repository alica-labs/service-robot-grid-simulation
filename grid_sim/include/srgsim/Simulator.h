#pragma once

#include "container/Command.h"

#include "GUI.h"
#include "SRGEnums.h"
#include "World.h"

#include <essentials/IDConstPtr.h>

#include <queue>
#include <unordered_map>

namespace std
{
class thread;
}

namespace srgsim
{
class Object;
class Simulator
{
public:
    Simulator(bool headless = false);
    ~Simulator();
    void start();
    void run();
    void initWorld();
    static bool isRunning();

    void spawnRobot(essentials::ID* id);
    void moveObject(essentials::ID* id, Direction direction);

    static std::string getSelfPath();

    static void simSigintHandler(int sig);

private:
    Cell* getNeighbourCell(const Direction& direction, Object* object);
    bool isPlacementAllowed(Cell* cell, Type objectType);

    static bool running;
    bool headless;
    World* world;
    GUI* gui;

    std::unordered_map<essentials::IDConstPtr, Object*> objects;

    std::thread* mainThread;
    std::queue<Command> commandQueue;
};
} // namespace srgsim
