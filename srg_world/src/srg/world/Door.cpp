#include "srg/world/Door.h"

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
Door::Door(essentials::IdentifierConstPtr id, ObjectState state)
        : Object(id, ObjectType::Door, state, 0)
{
}

bool Door::isOpen() const
{
    return this->state == ObjectState::Open;
};

std::ostream& operator<<(std::ostream& os, const Door& door) {
    os << "[Door] ID: " << door.id << " State: " <<  door.state;
    return os;
}

} // namespace world
} // namespace srg