#pragma once

namespace srgsim
{

enum Type
{
    Dirt,
    Gras,
    Wall,
    Door,
    Unknown,
    Floor,
    Robot,
    White,
    Default,
    Last // has to be last for iterating with ints over this enum
};

enum Direction
{
    Left,
    Up,
    Down,
    Right
};
} // namespace srgsim