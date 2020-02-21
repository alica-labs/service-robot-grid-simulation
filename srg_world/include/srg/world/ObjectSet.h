#pragma once

#include <essentials/IdentifierConstPtr.h>

#include <memory>
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

    const std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>& getObjects() const;
    virtual bool addObject(std::shared_ptr<world::Object> object);
    virtual void removeObject(std::shared_ptr<world::Object> object);
    virtual std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::iterator removeObject(
            std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>>::iterator iter);
    virtual void update(std::vector<std::shared_ptr<world::Object>> objects);
    virtual bool contains(std::shared_ptr<const world::Object> object) const;
    virtual bool contains(essentials::IdentifierConstPtr objectID) const;
    friend ::srg::World;
    friend std::ostream& operator<<(std::ostream& os, const ObjectSet& objectSet);

protected:
    explicit ObjectSet(int32_t capacity = INT32_MAX);
    int32_t capacity;
    std::unordered_map<essentials::IdentifierConstPtr, std::shared_ptr<world::Object>> containingObjects;
};
} // namespace world
} // namespace srg