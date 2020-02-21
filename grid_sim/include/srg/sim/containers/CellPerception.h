#pragma once

#include "srg/world/Object.h"

namespace srg
{
namespace sim
{
namespace containers
{
struct CellPerception
{
    uint32_t x;
    uint32_t y;
    std::vector<std::shared_ptr<srg::world::Object>> objects;
    friend std::ostream& operator<<(std::ostream& os, const CellPerception& obj)
    {
        os << "[CellPerception] At (" << obj.x << ", " << obj.y << ")" << std::endl;
        for (std::shared_ptr<srg::world::Object> o : obj.objects) {
            os << *o;
        }
        return os;
    }

};
} // namespace containers
} // namespace sim
} // namespace srg