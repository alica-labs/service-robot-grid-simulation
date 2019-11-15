#include "srg/world/Door.h"

#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
Door::Door(essentials::IdentifierConstPtr id, ObjectState state)
        : Object(ObjectType::Door, id, state)
{
}

bool Door::isOpen() const
{
    return this->state == ObjectState::Open;
};

} // namespace world
} // namespace srg