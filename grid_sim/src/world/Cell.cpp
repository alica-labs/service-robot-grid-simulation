#include "srgsim/world/Cell.h"

#include "srgsim/world/Object.h"
#include "srgsim/world/Room.h"

#include <iostream>

namespace srgsim
{
Cell::Cell(uint32_t x, uint32_t y)
        : coordinate(x, y)
        , up(nullptr)
        , down(nullptr)
        , left(nullptr)
        , right(nullptr)
        , room(nullptr)
{
}

Cell::Cell(const srgsim::Cell& cell)
        : coordinate(cell.coordinate)
{
    this->room = cell.room;
    this->up = cell.up;
    this->down = cell.down;
    this->left = cell.left;
    this->right = cell.right;
    this->objects = cell.objects;
}

RoomType Cell::getType() const
{
    return this->room->getType();
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

void Cell::update(std::vector<Object*> updateObjects)
{
    // remove unseen objects
    for (Object* cellObject : this->objects) {
        bool found = false;
        for (Object* updateObject : updateObjects) {
            if (updateObject->getID() == cellObject->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            this->removeObject(cellObject);
        }
    }

    // add new objects
    for (Object* updateObject : updateObjects) {
        bool found = false;
        for (Object* cellObject : this->objects) {
            if (updateObject->getID() == cellObject->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            this->addObject(updateObject);
        }
    }
}

bool Cell::operator<(const Cell* other) {
    return this->coordinate < other->coordinate;
}

bool operator==(Cell const& first, Cell const& second)
{
    return first.coordinate == second.coordinate;
}

bool operator!=(Cell const& first, Cell const& second)
{
    return !(first == second);
}

std::ostream& operator<<(std::ostream& os, const srgsim::Cell& obj)
{
    os << "[Cell] " << obj.coordinate << " Type: " << obj.getType() << " " << *(obj.room) << std::endl;
    return os;
}

} // namespace srgsim
