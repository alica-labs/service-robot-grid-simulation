#pragma once

#include "Object.h"

namespace srgsim
{
class Door : public Object
{
public:
    Door(essentials::IdentifierConstPtr id, ObjectState state = ObjectState::Closed);
    bool isOpen() const;
};
} // namespace srgsim
