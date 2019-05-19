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
    Object(Type type);
    virtual ~Object();

    void setCell(Cell* cell);
    Cell* getCell();
    Type getType() const;
    essentials::IDConstPtr getID();

private:
    Cell* cell;
    Type type;
    essentials::ID* id;
    static int objectCounter;

};

} // namespace srgsim
