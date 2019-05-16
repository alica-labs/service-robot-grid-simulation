#pragma once

#include <stdint.h>

namespace grid_sim
{

class Coordinate
{
public:
    Coordinate(uint32_t x, uint32_t y);
    virtual ~Coordinate() = default;
    Coordinate(const Coordinate& coordinate);

    uint32_t x;
    uint32_t y;
};

bool operator==(Coordinate const& first, Coordinate const& second);
} // namespace grid_sim