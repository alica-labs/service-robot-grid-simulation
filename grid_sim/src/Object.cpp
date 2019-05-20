#include "srgsim/Object.h"

#include "srgsim/Cell.h"

#include <essentials/ID.h>

namespace srgsim
{

Object::Object(Type type, const essentials::ID* id)
        : type(type), id(id)
{
}

Object::~Object()
{
    delete this->id;
}

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

Type Object::getType() const
{
    return type;
}

essentials::IDConstPtr Object::getID()
{
    return essentials::IDConstPtr(this->id);
}
} // namespace srgsim
