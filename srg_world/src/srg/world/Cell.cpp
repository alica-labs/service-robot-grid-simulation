#include "srg/world/Cell.h"

#include "srg/world/Object.h"
#include "srg/world/Room.h"

#include <iostream>

namespace srg
{
namespace world
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

RoomType Cell::getType() const
{
    return this->room->getType();
}

bool Cell::isBlocked() const
{
    if (this->getType() == srg::world::RoomType::Wall) {
        return true;
    }
    for (auto object : this->objects) {
        if (object->getType() == srg::world::ObjectType::Door) {
            return true;
        }
    }
    return false;
}

bool Cell::contains(essentials::IdentifierConstPtr objectID) const
{
    for (const Object* object : this->objects) {
        if (object->getID() == objectID) {
            return true;
        }
    }
    return false;
}

bool Cell::contains(const Object* object) const
{
    return this->contains(object->getID());
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

bool Cell::operator<(const Cell* other)
{
    return this->coordinate < other->coordinate;
}

bool operator==(Cell const& first, Cell const& second)
{
    return first.coordinate == second.coordinate;
}

bool Cell::operator==(Cell const* other)
{
    return this->coordinate == other->coordinate;
}

bool operator!=(Cell const& first, Cell const& second)
{
    return !(first == second);
}

std::ostream& operator<<(std::ostream& os, const Cell& obj)
{
    os << "[Cell] " << obj.coordinate << " Type: " << obj.getType() << " " << *(obj.room) << std::endl;
    return os;
}
} // namespace world
} // namespace srg
