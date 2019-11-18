#include "srg/world/ServiceRobot.h"

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
ServiceRobot::ServiceRobot(essentials::IdentifierConstPtr id)
        : Object(ObjectType::Robot, id)
        , carriedObject(nullptr)
{
}
bool ServiceRobot::isCarrying(essentials::IdentifierConstPtr id) const
{
    return carriedObject && carriedObject->getID() == id;
}

Object* ServiceRobot::getCarriedObject()
{
    return this->carriedObject;
}

void ServiceRobot::setCarriedObject(Object* object)
{
    this->carriedObject = object;
}

std::ostream& operator<<(std::ostream& os, const ServiceRobot& obj)
{
    if (obj.carriedObject) {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " Carrying: " << *obj.carriedObject << std::endl;
    } else {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " doesnt carry anything! " << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg
