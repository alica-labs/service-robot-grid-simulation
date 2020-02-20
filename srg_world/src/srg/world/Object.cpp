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

Coordinate Object::getCoordinate() const {
    const ObjectSet* parentContainer = this->parentContainer;
    const srg::world::Cell* cell = nullptr;
    const srg::world::Object* object = nullptr;

    cell = dynamic_cast<const srg::world::Cell*>(parentContainer);
    if (cell){
        return cell->coordinate;
    } else {
        object = dynamic_cast<const srg::world::Object*>(parentContainer);
        if (object) {
            // recursive call, because objects can be contained in other objects
            // example: a robot holds and object
            return object->getCoordinate();
        }
    }
    std::cerr << "[Object] Object has no coordinates! " << *this << std::endl;
    return Coordinate(-1,-1);
}

void Object::deleteParentContainer()
{
    if (this->parentContainer == nullptr) {
        return;
    }
    ObjectSet* tmpContainer = this->parentContainer;
    this->parentContainer = nullptr;
    tmpContainer->removeObject(this);
}

void Object::setParentContainer(ObjectSet* parentContainer)
{
    if (parentContainer == nullptr || this->parentContainer == parentContainer) {
        return;
    }
    this->deleteParentContainer();
    this->parentContainer = parentContainer;
    this->parentContainer->addObject(this);
}

const ObjectSet* Object::getParentContainer() const
{
    return this->parentContainer;
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