#pragma once

#include "container/Command.h"

#include "World.h"
#include "GUI.h"

#include <essentials/IDConstPtr.h>

#include <queue>
#include <unordered_map>

namespace std {
    class thread;
}

namespace srgsim {
    class Object;
    class Simulator {
    public:
        Simulator(bool headless = false);
        ~Simulator();
        void initWorld();
        void spawnRobot(essentials::ID* id);

        void start();
        void run();

        static std::string getSelfPath();
        static void simSigintHandler(int sig);
        static bool isRunning();
        static bool running;

    private:
        bool headless;
        World* world;
        GUI* gui;

        std::unordered_map<essentials::IDConstPtr, Object*> objects;

        std::thread* mainThread;
        std::queue<Command> commandQueue;
    };
}
