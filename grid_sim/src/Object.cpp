#include "srgsim/Object.h"

#include "srgsim/Cell.h"

#include <essentials/ID.h>

namespace srgsim
{

int Object::objectCounter = 0;

Object::Object(Type type)
        : type(type)
{
    // little-endian encoding
    // TODO replace with id manager(refactored agentID manager)
    std::vector<uint8_t> idByteVector;
    for (int i = 0; i < static_cast<int>(sizeof(objectCounter)); i++) {
        idByteVector.push_back(*(((uint8_t*) &objectCounter) + i));
    }
    this->id = new essentials::ID(idByteVector.data(), idByteVector.size());
    objectCounter++;
}

Object::~Object()
{
    delete this->id;
}

void Object::setCell(Cell* cell)
{
    this->cell = cell;
}

Cell* Object::getCell()
{
    return this->cell;
}

Type Object::getType() const
{
    return type;
}

essentials::IDConstPtr Object::getID() {
    return essentials::IDConstPtr(this->id);
}
} // namespace srgsim
