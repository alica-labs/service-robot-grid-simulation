#include "srg/world/Object.h"

#include "srg/world/Cell.h"

namespace srg
{
namespace world
{
Object::Object(ObjectType type, essentials::IdentifierConstPtr id, ObjectState state)
        : type(type)
        , state(state)
        , id(id)
        , cell(nullptr)
{
}

Object::~Object() {}

void Object::setCell(Cell* cell)
{
    if (this->cell != nullptr) {
        if (this->cell == cell)
            return;
        this->cell->removeObject(this);
    }
    this->cell = cell;
    this->cell->addObject(this);
}

const Cell* Object::getCell() const
{
    return this->cell;
}

Cell* Object::editCell()
{
    return this->cell;
}

void Object::deleteCell()
{
    if (!this->cell)
        return;
    Cell* tmp = this->cell;
    this->cell = nullptr;
    tmp->removeObject(this);
}

ObjectType Object::getType() const
{
    return type;
}

void Object::setType(ObjectType type)
{
    this->type = type;
}

ObjectState Object::getState() const
{
    return state;
}

void Object::setState(ObjectState state)
{
    this->state = state;
}

essentials::IdentifierConstPtr Object::getID() const
{
    return essentials::IdentifierConstPtr(this->id);
}

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
    os << "ID: " << obj.id << " Type: " << obj.type << " State: " << obj.state << std::endl;
    return os;
}
} // namespace world
} // namespace srg