#include <srg/world/Agent.h>
#include "srg/world/Object.h"

#include "srg/world/Cell.h"

namespace srg
{
namespace world
{
Object::Object(essentials::IdentifierConstPtr id, ObjectType type, ObjectState state, int32_t capacity)
        : ObjectSet(capacity)
        , type(type)
        , state(state)
        , id(id)
        , parentContainer(nullptr)
{
}

Object::~Object() {}

Coordinate Object::getCoordinate() const
{
    std::shared_ptr<const ObjectSet> parentContainer = this->parentContainer;
    std::shared_ptr<const Cell> cell = nullptr;
    std::shared_ptr<const Object> object = nullptr;

    cell = std::dynamic_pointer_cast<const srg::world::Cell>(parentContainer);
    if (cell) {
        return cell->coordinate;
    } else {
        object = std::dynamic_pointer_cast<const srg::world::Object>(parentContainer);
        if (object) {
            // recursive call, because objects can be contained in other objects
            // example: a robot holds and object
            return object->getCoordinate();
        }
    }
    //    std::cerr << "[Object] Object has no coordinates! " << *this << std::endl;
    return Coordinate(-1, -1);
}

void Object::deleteParentContainer()
{
    if (this->parentContainer == nullptr) {
        return;
    }
    std::shared_ptr<ObjectSet> tmpContainer = this->parentContainer;
    this->parentContainer = nullptr;
    tmpContainer->removeObject(std::dynamic_pointer_cast<world::Object>(this->shared_from_this()));
}

void Object::setParentContainer(std::shared_ptr<ObjectSet> parentContainer)
{
    if (parentContainer == nullptr || this->parentContainer == parentContainer) {
        return;
    }
    this->deleteParentContainer();
    this->parentContainer = parentContainer;
    this->parentContainer->addObject(std::dynamic_pointer_cast<world::Object>(this->shared_from_this()));
}

std::shared_ptr<const ObjectSet> Object::getParentContainer() const
{
    return this->parentContainer;
}

bool Object::canBePickedUp(essentials::IdentifierConstPtr agentID) const
{
    switch (type) {
    case ObjectType::Human:
    case ObjectType::Robot:
    case ObjectType::Door:
    case ObjectType::Unknown:
        return false;
    case ObjectType::CupBlue:
    case ObjectType::CupYellow:
    case ObjectType::CupRed:
        if (std::dynamic_pointer_cast<Cell>(this->parentContainer)
                || std::dynamic_pointer_cast<Agent>(this->parentContainer)->getID() == agentID) {
            // The object is layed down, or is picked by the given agent already
            return true;
        } else {
            // The object is carried already
            return false;
        }
    }
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
    return this->id;
}

std::ostream& operator<<(std::ostream& os, const Object& obj)
{
    os << "[Object] " << obj.type << "(" << obj.id << ") State: " << obj.state << " Contained Objects (Size " << obj.containingObjects.size()
       << "):" << std::endl;
    for (auto& objectEntry : obj.containingObjects) {
        os << "\t" << *objectEntry.second << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg