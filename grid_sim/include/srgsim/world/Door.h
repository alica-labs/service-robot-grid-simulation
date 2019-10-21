#pragma once

#include "Object.h"

namespace srgsim
{
class Door : public Object
{
public:
    Door(essentials::IdentifierConstPtr id, bool open);
    void setOpen(bool open);
    bool isOpen();
private:
    bool open;
};
} // namespace srgsim
