#include "srgsim/world/ServiceRobot.h"

#include "srgsim/world/Localisation.h"
#include "srgsim/world/ObjectDetection.h"
#include "srgsim/world/Manipulation.h"

#include <essentials/IdentifierConstPtr.h>

namespace srgsim{

    ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id) : Object(Type::Robot, id) {
        this->localisation = new Localisation(this);
        this->objectDetection = new ObjectDetection(this);
        this->manipulation = new Manipulation(this);
    }

    SimPerceptions ServiceRobot::createSimPerceptions(World* world) {
        SimPerceptions sps;
        sps.receiverID = this->getID();

        // localisation
        std::vector<Perception> localisationPerceptions = this->localisation->createPerceptions(world);
        sps.perceptions.insert(sps.perceptions.begin(), localisationPerceptions.begin(), localisationPerceptions.end());

        // objects
        std::vector<Perception> objectPerceptions = this->objectDetection->createPerceptions(world);
        sps.perceptions.insert(sps.perceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

        return sps;
    }

    void ServiceRobot::executeAction(SimCommand sc, World* world) {
        this->manipulation->manipulate(sc, world);
    }

    Object *ServiceRobot::getCarriedObject() {
        return this->manipulation->getCarriedObject();
    }
}
