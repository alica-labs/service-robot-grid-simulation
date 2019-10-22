#pragma once

#include "Object.h"
#include "srgsim/containers/SimPerceptions.h"
#include "srgsim/containers/SimCommand.h"

namespace srgsim {
    class World;
    class Localisation;
    class ObjectDetection;
    class Manipulation;
    class ServiceRobot : public Object {
    public:
        ServiceRobot(essentials::IdentifierConstPtr id);
        SimPerceptions createSimPerceptions(World* world);
        void executeAction(SimCommand sc, World* world);

        Object *getCarriedObject();

    private:
        Localisation* localisation;
        ObjectDetection* objectDetection;
        Manipulation* manipulation;
    };
}
