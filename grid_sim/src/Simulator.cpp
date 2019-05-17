#include "Simulator.h"

#include <iostream>
#include <limits.h>
#include <signal.h>
#include <string>
#include <thread>
#include <unistd.h>
#include <Cell.h>

//#define SIM_DEBUG

namespace srgsim
{
bool Simulator::running = false;

Simulator::Simulator(bool headless)
        : headless(headless)
{
    this->initTestWorld();
}

Simulator::~Simulator()
{
    delete this->world;
    delete this->gui;
}

void Simulator::initTestWorld()
{
    this->world = new World();
    for (uint32_t i = 0; i < 10; i++) {
        for (uint32_t j = 0; j < 5; j++) {
            if(Cell* cell = this->world->addCell(i, j)) {
                cell->type = Type::Floor;
            }

        }
        for (uint32_t j = 5; j < 8; j++) {
            if(Cell* cell = this->world->addCell(i, j)) {
                cell->type = Type::Wall;
            }
        }
    }
    this->world->addCell(4,0)->type = Type::Unknown;
    this->world->addCell(5,0)->type = Type::Unknown;
    this->world->addCell(3,1)->type = Type::Unknown;
    this->world->addCell(6,1)->type = Type::Unknown;
    this->world->addCell(3,2)->type = Type::Unknown;
    this->world->addCell(6,2)->type = Type::Unknown;
    this->world->addCell(3,3)->type = Type::Unknown;
    this->world->addCell(6,3)->type = Type::Unknown;
    this->world->addCell(3,4)->type = Type::Unknown;
    this->world->addCell(6,4)->type = Type::Unknown;
    this->world->addCell(3,5)->type = Type::Unknown;
    this->world->addCell(6,5)->type = Type::Unknown;
    this->world->addCell(3,6)->type = Type::Unknown;
    this->world->addCell(6,6)->type = Type::Unknown;
    this->world->addCell(2,6)->type = Type::Unknown;
    this->world->addCell(7,6)->type = Type::Unknown;
    this->world->addCell(1,5)->type = Type::Unknown;
    this->world->addCell(8,5)->type = Type::Unknown;
    this->world->addCell(0,6)->type = Type::Unknown;
    this->world->addCell(9,6)->type = Type::Unknown;
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
    }
}

std::string Simulator::get_selfpath()
{
    char buff[PATH_MAX];
    size_t len = ::readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1) {
        buff[len] = '\0';
        std::string exePath = std::string(buff);

        return exePath.substr(0, exePath.find_last_of("/"));
    }
    /* handle error condition */
}

bool Simulator::isRunning()
{
    return running;
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
