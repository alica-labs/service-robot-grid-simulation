#pragma once
#include "World.h"
#include "GUI.h"

namespace std {
    class thread;
}

namespace grid_sim {
    class Simulator {
    public:
        Simulator();
        ~Simulator();
        void initTestWorld();

        void start();
        void run();

        static std::string get_selfpath();
        static void simSigintHandler(int sig);
        static bool isRunning();
        static bool running;

    private:
        World* world;
        GUI* gui;
        std::thread* mainThread;
    };
}
