#pragma once

#include "srg/world/Object.h"

namespace srg
{
namespace world
{
class Door : public Object
{
public:
    Door(essentials::IdentifierConstPtr id, ObjectState state = ObjectState::Closed);

    bool isOpen() const;
};
} // namespace world
} // namespace srg
