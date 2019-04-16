#pragma once

#include <string>

namespace grid_sim {

enum Type{
    Wall, Floor, Door
};

struct Cell {
    Type type;
    std::string room;
};

}