#include "srgsim/world/Door.h"

#include <essentials/IdentifierConstPtr.h>
#include <srgsim/SRGEnums.h>

namespace srgsim
{
Door::Door(essentials::IdentifierConstPtr id, bool open)
        : Object(Type::Door, id)
        , open(open)
{
}

bool Door::isOpen() {
    return open;
};

void Door::setOpen(bool open) {
    this->open = open;
}
} // namespace srgsim
