#pragma once

#include "container/Command.h"

#include "SRGEnums.h"

#include <id_manager/IdentifierConstPtr.h>

#include <queue>
#include <unordered_map>

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
class SRGIDManager;
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
    void initWorld();
    static bool isRunning();

    void spawnRobot(const essentials::Identifier* id);
    void moveObject(const essentials::Identifier* id, Direction direction);

    static std::string getSelfPath();

    static void simSigintHandler(int sig);

private:
    Cell* getNeighbourCell(const Direction& direction, Object* object);
    bool isPlacementAllowed(Cell* cell, Type objectType);

    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    communication::Communication* communication;

public:
    essentials::IDManager* getIdManager() const;

private:
    essentials::IDManager* idManager;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> objects;
    std::thread* mainThread;
    std::queue<container::Command> commandQueue;
};
} // namespace srgsim
