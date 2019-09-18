#pragma once

#include "srgsim/Object.h"

#include "srgsim/containers/SimPerceptions.h"

namespace srgsim {
    class World;
    class Localisation;
    class ServiceRobot : public Object {
    public:
        ServiceRobot(const essentials::Identifier* id);
        SimPerceptions createSimPerceptions(World* world);
    private:
        Localisation* localisation;
    };
}
