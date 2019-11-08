#include "srgsim/world/Door.h"

#include <essentials/IdentifierConstPtr.h>
#include <srgsim/world/TaskType.h>

namespace srgsim
{
Door::Door(essentials::IdentifierConstPtr id, ObjectState state)
        : Object(ObjectType::Door, id, state)
{
}

bool Door::isOpen() const {
    return this->state == ObjectState::Open;
};

} // namespace srgsim
