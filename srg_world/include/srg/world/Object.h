#pragma once

#include "srg/world/ObjectState.h"
#include "srg/world/ObjectType.h"

#include <essentials/Identifier.h>
#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{

class Cell;

class Object
{
public:
    Object(ObjectType type, essentials::IdentifierConstPtr id, ObjectState state = ObjectState::Undefined);

    virtual ~Object();

    essentials::IdentifierConstPtr getID() const;

    virtual ObjectType getType() const;

    void setType(ObjectType type);

    ObjectState getState() const;

    void setState(ObjectState state);

    const Cell* getCell() const;

    Cell* editCell();

    void setCell(Cell* cell);

    void deleteCell();

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

protected:
    Cell* cell;
    ObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};
} // namespace world
} // namespace srg
