#include "srg/world/Agent.h"

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
Agent::Agent(essentials::IdentifierConstPtr id, ObjectType type)
        : Object(id, type, ObjectState::Undefined, 1)
{
}

std::ostream& operator<<(std::ostream& os, const Agent& obj)
{
    if (obj.containingObjects.size() > 0) {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " Carrying: " << *obj.containingObjects.begin()->second;
    } else {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << " doesnt carry anything! ";
    }
    return os;
}
} // namespace world
} // namespace srg
