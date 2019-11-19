#pragma once

#include "srg/world/ObjectState.h"
#include "srg/world/ObjectType.h"
#include "srg/world/ObjectSet.h"

#include <essentials/Identifier.h>
#include <essentials/IdentifierConstPtr.h>

namespace srg
{
namespace world
{
class Object : public ObjectSet
{
public:
    Object(ObjectType type, essentials::IdentifierConstPtr id, ObjectState state = ObjectState::Undefined, int32_t capacity = INT32_MAX);
    ~Object() override;

    essentials::IdentifierConstPtr getID() const;

    virtual ObjectType getType() const;
    void setType(ObjectType type);

    ObjectState getState() const;
    void setState(ObjectState state);

    const ObjectSet* getParentContainer() const;
    void setParentContainer(ObjectSet* parentContainer);
//    void deleteCell();

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);
protected:
    ObjectSet* parentContainer; /**< ServiceRobot, Cell, etc.*/
    ObjectType type;
    ObjectState state;
    essentials::IdentifierConstPtr id;
};
} // namespace world
} // namespace srg
