#include "srg/world/Object.h"

namespace srg
{
namespace world
{
Object::Object(ObjectType type, essentials::IdentifierConstPtr id, ObjectState state, int32_t capacity)
        : ObjectSet(capacity)
        , type(type)
        , state(state)
        , id(id)
        , parentContainer(nullptr)
{
}

Object::~Object() {}

void Object::setParentContainer(ObjectSet* parentContainer)
{
    if (this->parentContainer != nullptr) {
        if (this->parentContainer == parentContainer)
            return;
        this->parentContainer->removeObject(this);
    }
    this->parentContainer = parentContainer;
    if (this->parentContainer) {
        this->parentContainer->addObject(this);
    }
}

const ObjectSet* Object::getParentContainer() const
{
    return this->parentContainer;
}

//void Object::deleteCell()
//{
//    if (!this->parentContainer)
//        return;
//    ObjectSet* tmp = this->parentContainer;
//    this->parentContainer = nullptr;
//    tmp->removeObject(this);
//}

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
    os << "[Object] " << obj.type << "(" << obj.id << ") State: " << obj.state << " Contained Objects (Size " << obj.containingObjects.size() << "):" << std::endl;
    for (auto& objectEntry : obj.containingObjects) {
        os << "\t" << *objectEntry.second << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg