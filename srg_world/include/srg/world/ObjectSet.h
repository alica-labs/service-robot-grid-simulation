#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <unordered_map>

namespace srg
{
class World;
namespace world
{
class Object;
class ObjectSet
{
public:
    virtual ~ObjectSet();

    const std::unordered_map<essentials::IdentifierConstPtr, Object*>& getObjects() const;
    virtual bool addObject(Object* object);
    virtual void removeObject(Object* object);
    virtual void update(std::vector<Object*> objects);
    virtual bool contains(const Object* object) const;
    virtual bool contains(essentials::IdentifierConstPtr objectID) const;
    friend ::srg::World;
    friend std::ostream& operator<<(std::ostream& os, const ObjectSet& objectSet);
protected:
    explicit ObjectSet(int32_t capacity = INT32_MAX);
    int32_t capacity;
    std::unordered_map<essentials::IdentifierConstPtr, Object*> containingObjects;
};
} // namespace world
} // namespace srg