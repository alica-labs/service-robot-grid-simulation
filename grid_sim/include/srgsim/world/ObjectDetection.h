#pragma once

#include "srgsim/containers/Perception.h"
#include "Sensor.h"

#include <vector>

namespace srgsim {
    class ObjectDetection : public Sensor{
    public:
        ObjectDetection(ServiceRobot* robot);
        std::vector<Perception> createPerceptions(World* world);
    };
}