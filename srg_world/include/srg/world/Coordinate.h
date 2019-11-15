#pragma once

#include <iosfwd>
#include <stdint.h>

namespace srg
{
namespace world
{

class Coordinate
{
public:
    Coordinate(int32_t x, int32_t y);

    virtual ~Coordinate();

    Coordinate(const Coordinate& coordinate);

    int32_t x;
    int32_t y;

    Coordinate abs();

    friend std::ostream& operator<<(std::ostream& os, const Coordinate& obj);
};

bool operator==(Coordinate const& first, Coordinate const& second);

bool operator!=(Coordinate const& first, Coordinate const& second);

bool operator<(Coordinate const& first, Coordinate const& second);

Coordinate operator-(Coordinate const& first, Coordinate const& second);
} // namespace world
} // namespace srg