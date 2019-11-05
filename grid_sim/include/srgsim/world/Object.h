#pragma once

#include "srgsim/world/SpriteObjectType.h"
#include "srgsim/world/ObjectState.h"

#include <essentials/IdentifierConstPtr.h>
#include <essentials/Identifier.h>

namespace srgsim
{

class Cell;

class Object
{
public:
    Object(SpriteObjectType type, essentials::IdentifierConstPtr id, ObjectState state = ObjectState::Undefined);
    virtual ~Object();

    essentials::IdentifierConstPtr getID() const;
    SpriteObjectType getType() const;
    void setType(SpriteObjectType type);
    ObjectState getState() const;
    void setState(ObjectState state);

    Cell* getCell();
    void setCell(Cell* cell);
    void deleteCell();

    friend std::ostream& operator<<(std::ostream& os, const srgsim::Object& obj)
    {
        os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << std::endl;
        return os;
    }
protected:
    Cell* cell;
    SpriteObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};

} // namespace srgsim
