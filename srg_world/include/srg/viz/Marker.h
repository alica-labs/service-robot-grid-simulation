#pragma once

#include "srg/viz/SpriteType.h"

#include <srg/world/Coordinate.h>

namespace srg
{
namespace viz
{
class Marker
{
public:
    Marker(world::Coordinate coordinate);

    SpriteType type;
    world::Coordinate coordinate;
};
} // namespace viz
} // namespace srg