#include "srgsim/world/Object.h"

#include "srgsim/world/Cell.h"

namespace srgsim
{

Object::Object(SpriteObjectType type, essentials::IdentifierConstPtr id, ObjectState state)
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

Cell* Object::getCell()
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

SpriteObjectType Object::getType() const
{
    return type;
}

void Object::setType(SpriteObjectType type)
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
} // namespace srgsim
