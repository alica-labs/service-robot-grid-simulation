#pragma once

#include "srgsim/SRGEnums.h"

#include <essentials/IDConstPtr.h>

namespace essentials
{
class ID;
}

namespace srgsim
{

class Cell;

class Object
{
public:
    Object(Type type, const essentials::ID* id);
    virtual ~Object();

    essentials::IDConstPtr getID();
    Type getType() const;

    Cell* getCell();
    void setCell(Cell* cell);
    void deleteCell();

private:
    Cell* cell;
    Type type;
    const essentials::ID* id;
};

} // namespace srgsim
