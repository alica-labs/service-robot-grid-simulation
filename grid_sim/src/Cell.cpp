#include "srgsim/Cell.h"

#include "srgsim/Object.h"

#include <iostream>

namespace srgsim
{
Cell::Cell(uint32_t x, uint32_t y)
        : coordinate(x, y)
{
    this->up = nullptr;
    this->down = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->type = Type::Unknown;
}

Cell::Cell(const srgsim::Cell& cell)
        : coordinate(cell.coordinate)
{
    this->room = cell.room;
    this->type = cell.type;
    this->up = cell.up;
    this->down = cell.down;
    this->left = cell.left;
    this->right = cell.right;
    this->objects = cell.objects;
}

const std::vector<Object*>& Cell::getObjects() const
{
    return objects;
}

void Cell::addObject(Object* object)
{
    for (Object* o : objects) {
        if (o->getID() == object->getID()) {
            return;
        }
    }
    objects.push_back(object);
    object->setCell(this);
}
void Cell::removeObject(Object* object)
{
    for (size_t i = 0; i < objects.size(); i++) {
        if (objects.at(i)->getID() == object->getID()) {
            objects.erase(objects.begin() + i);
            object->deleteCell();
            return;
        }
    }
}

} // namespace srgsim
