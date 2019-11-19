#include "srg/world/ServiceRobot.h"

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id)
        : Object(ObjectType::Robot, id, ObjectState::Undefined, 1)
{
}
bool ServiceRobot::isCarrying(essentials::IdentifierConstPtr id) const
{
    return this->contains(id);
}

Object* ServiceRobot::getCarriedObject()
{
    if (this->containingObjects.size() > 0) {
        return this->containingObjects.begin()->second;
    } else {
        return nullptr;
    }
}

void ServiceRobot::setCarriedObject(Object* object)
{
    this->addObject(object);
}

std::ostream& operator<<(std::ostream& os, const ServiceRobot& obj)
{
    if (obj.containingObjects.size() > 0) {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " Carrying: " << *obj.containingObjects.begin()->second << std::endl;
    } else {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " doesnt carry anything! " << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg
