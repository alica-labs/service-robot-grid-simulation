#pragma once

#include "srgsim/containers/Perception.h"

#include <vector>

namespace srgsim {
    class World;
    class ServiceRobot;
    class Sensor {
    public:
        Sensor(ServiceRobot* robot);
        virtual ~Sensor();
        virtual std::vector<Perception> createPerceptions(World* world) = 0;
    protected:
        ServiceRobot* robot;
    };
}
