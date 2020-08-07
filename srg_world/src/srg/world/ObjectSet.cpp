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

bool ObjectSet::contains(std::shared_ptr<const world::Object> object) const
{
    return this->contains(object->getID());
}

const std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>& ObjectSet::getObjects() const
{
    return containingObjects;
}

bool ObjectSet::addObject(std::shared_ptr<world::Object> object)
{
    if (this->containingObjects.size() < capacity) {
        if (this->containingObjects.insert({object->getID(), object}).second) {
            object->setParentContainer(this->shared_from_this());
            return true;
        }
    }
    return false;
}

void ObjectSet::removeObject(std::shared_ptr<world::Object> object)
{
    if (this->containingObjects.erase(object->getID()) > 0) {
        object->deleteParentContainer();
    }
}

std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::iterator ObjectSet::removeObject(
        std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::iterator iter)
{
    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::iterator iterator = this->containingObjects.erase(iter);
    iter->second->deleteParentContainer();
    return iterator;
}

void ObjectSet::update(std::vector<std::shared_ptr<world::Object>> updateObjects)
{
    // remove unseen objects
    for (auto it = this->containingObjects.begin(); it != this->containingObjects.end();) {
        bool found = false;
        for (std::shared_ptr<world::Object> updateObject : updateObjects) {
            if (updateObject->getID() == it->second->getID()) {
                found = true;
                break;
            }
        }
        if (!found) {
            it = this->removeObject(it);
        } else {
            it++;
        }
    }

    // add new objects
    for (std::shared_ptr<world::Object> updateObject : updateObjects) {
        this->addObject(updateObject);
    }
}

std::ostream& operator<<(std::ostream& os, const ObjectSet& objectSet)
{
    for (auto& objectEntry : objectSet.containingObjects) {
        os << *objectEntry.second << std::endl;
    }
    return os;
}
} // namespace world
} // namespace srg
