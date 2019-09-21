#pragma once

#include "srgsim/containers/Perception.h"
#include "Sensor.h"

#include <vector>

namespace srgsim {
    class Localisation : public Sensor{
    public:
        Localisation(ServiceRobot* robot);
        std::vector<Perception> createPerceptions(World* world);
    };
}