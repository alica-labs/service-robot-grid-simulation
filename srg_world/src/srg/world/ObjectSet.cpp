#include "srg/world/ObjectSet.h"

#include "srg/world/Object.h"

namespace srg
{
namespace world
{
ObjectSet::ObjectSet(int32_t capacity)
        : capacity(capacity)
{
}

ObjectSet::~ObjectSet() {}

bool ObjectSet::contains(essentials::IdentifierConstPtr objectID) const
{
    return this->containingObjects.find(objectID) != this->containingObjects.end();
}

bool ObjectSet::contains(const Object* object) const
{
    return this->contains(object->getID());
}

const std::unordered_map<essentials::IdentifierConstPtr, Object*>& ObjectSet::getObjects() const
{
    return containingObjects;
}

bool ObjectSet::addObject(Object* object)
{
    if (this->containingObjects.size() < capacity) {
        if (this->containingObjects.insert({object->getID(), object}).second) {
            object->setParentContainer(this);
        }
    }
}

void ObjectSet::removeObject(Object* object)
{
    if (this->containingObjects.erase(object->getID()) > 0) {
        object->setParentContainer(nullptr);
    }
}

void ObjectSet::update(std::vector<Object*> updateObjects)
{
    // remove unseen objects
    for (auto it = this->containingObjects.begin(); it != this->containingObjects.end();) {
        std::cout << "[ObjectSet] Size " << this->containingObjects.size() << std::endl;
        bool found = false;
        for (Object* updateObject : updateObjects) {
            std::cout << "[ObjectSet] " << *updateObject << std::endl;
            if (updateObject->getID() == it->second->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            // this already deletes the object from this container... :D
            it->second->setParentContainer(nullptr);
            it = this->containingObjects.erase(it);
        } else {
            it++;
        }
    }

    // add new objects
    for (Object* updateObject : updateObjects) {
        this->addObject(updateObject);
    }
}

std::ostream& operator<<(std::ostream& os, const ObjectSet& objectSet) {
    for (auto& objectEntry : objectSet.containingObjects) {
        os << *objectEntry.second << std::endl;
    }
}
} // namespace world
} // namespace srg
