#include "srgsim/world/ServiceRobot.h"

#include "srgsim/world/Manipulation.h"
#include "srgsim/world/ObjectDetection.h"

#include <essentials/IdentifierConstPtr.h>

namespace srgsim
{

ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id)
        : Object(SpriteObjectType::Robot, id)
{
    this->objectDetection = new ObjectDetection(this);
    this->manipulation = new Manipulation(this);
}

SimPerceptions ServiceRobot::createSimPerceptions(World* world)
{
    SimPerceptions sps;
    sps.receiverID = this->getID();

    // objects
    std::vector<CellPerceptions> objectPerceptions = this->objectDetection->createPerceptions(world);
    sps.cellPerceptions.insert(sps.cellPerceptions.end(), objectPerceptions.begin(), objectPerceptions.end());

    return sps;
}

void ServiceRobot::executeAction(SimCommand sc, World* world)
{
    this->manipulation->manipulate(sc, world);
}

const Object* ServiceRobot::getCarriedObject() const
{
    return this->manipulation->getCarriedObject();
}

std::ostream& operator<<(std::ostream& os, const srgsim::ServiceRobot& obj)
{
    Object* carriedObj = obj.manipulation->getCarriedObject();
    if (carriedObj) {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " Carrying: "
           << carriedObj << std::endl;
    } else {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " doesnt carry anything! " << std::endl;
    }
    return os;
}
} // namespace srgsim
