#pragma once

#include "containers/SimCommand.h"

#include "SRGEnums.h"

#include <essentials/IdentifierConstPtr.h>

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
    Object* addObject(const essentials::Identifier* id, Type type);
    void moveObject(const essentials::Identifier* id, Direction direction);
    void pickObject(const essentials::Identifier* id, Direction direction);
    void putObject(const essentials::Identifier* id, Direction direction);
    essentials::IDManager* getIdManager() const;

    static void simSigintHandler(int sig);
    std::string getSelfPath();

private:
    Cell* getNeighbourCell(const Direction& direction, Object* object);
    bool isPlacementAllowed(Cell* cell, Type objectType);

    static bool running;
    bool headless;
    World* world;
    GUI* gui;
    communication::Communication* communication;

    essentials::IDManager* idManager;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> objects;
    std::thread* mainThread;
    std::queue<SimCommand> commandQueue;

};
} // namespace srgsim
