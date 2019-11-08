#pragma once

#include "srgsim/world/ObjectState.h"
#include "srgsim/world/ObjectType.h"

#include <essentials/IdentifierConstPtr.h>
#include <essentials/Identifier.h>

namespace srgsim
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

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Object& obj)
    {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << std::endl;
        return os;
    }
protected:
    Cell* cell;
    ObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};

} // namespace srgsim
