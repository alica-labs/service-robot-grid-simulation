#pragma once

#include "srg/world/ObjectState.h"
#include "srg/world/ObjectType.h"
#include "srg/world/ObjectSet.h"
#include "Coordinate.h"

#include <essentials/Identifier.h>
#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
class Object : public ObjectSet, std::enable_shared_from_this<Object>
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

    const ObjectSet* getParentContainer() const;
    void setParentContainer(ObjectSet* parentContainer);
    void deleteParentContainer();

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
protected:
    ObjectSet* parentContainer; /**< ServiceRobot, Cell, etc.*/
    ObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};
} // namespace world
} // namespace srg
