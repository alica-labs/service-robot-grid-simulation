#pragma once

#include "srg/world/Object.h"

namespace srg
{
class World;
namespace world
{
class Agent : public Object
{
public:
    explicit Agent(essentials::IdentifierConstPtr id, ObjectType type);
    // friend declarations
    friend World;

    friend std::ostream& operator<<(std::ostream& os, const Agent& obj);

private:
};
} // namespace world
} // namespace srg
