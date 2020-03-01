#pragma once

#include "srg/world/ObjectState.h"
#include "srg/world/ObjectType.h"
#include "srg/world/ObjectSet.h"
#include "Coordinate.h"

#include <essentials/Identifier.h>
#include <essentials/IdentifierConstPtr.h>

#include <memory>

namespace srg
{
namespace world
{
class Object : public ObjectSet
{
public:
    Object(essentials::IdentifierConstPtr id, ObjectType type, ObjectState state = ObjectState::Undefined, int32_t capacity = INT32_MAX);
    ~Object() override;

    essentials::IdentifierConstPtr getID() const;

    virtual ObjectType getType() const;
    void setType(ObjectType type);

    ObjectState getState() const;
    void setState(ObjectState state);

    Coordinate getCoordinate() const;

    std::shared_ptr<const ObjectSet> getParentContainer() const;
    void setParentContainer(std::shared_ptr<ObjectSet> parentContainer);
    void deleteParentContainer();

    bool canBePickedUp() const;

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
protected:
    std::shared_ptr<ObjectSet> parentContainer; /**< ServiceRobot, Cell, etc.*/
    ObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};
} // namespace world
} // namespace srg
