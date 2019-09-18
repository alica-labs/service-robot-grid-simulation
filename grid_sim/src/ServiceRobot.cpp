#include "srgsim/ServiceRobot.h"

#include "srgsim/Localisation.h"
#include <essentials/IdentifierConstPtr.h>

namespace srgsim{

    ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id) : Object(Type::Robot, id) {
        this->localisation = new Localisation(this);
    }

    SimPerceptions ServiceRobot::createSimPerceptions(World *world) {
        SimPerceptions sps;
        sps.receiverID = this->getID();

        // localisation
        std::vector<Perception> localisationPerceptions = this->localisation->createPerceptions(world);
        sps.perceptions.insert(sps.perceptions.begin(), localisationPerceptions.begin(), localisationPerceptions.end());

        // objects
        // TODO

        return sps;
    }
}
