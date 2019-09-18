#include "srgsim/Sensor.h"

#include "srgsim/ServiceRobot.h"

namespace srgsim
{
Sensor::Sensor(ServiceRobot* robot)
        : robot(robot)
{
}

Sensor::~Sensor() {}
} // namespace srgsim
