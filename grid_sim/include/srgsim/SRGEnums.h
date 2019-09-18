#pragma once

namespace srgsim
{
/**
 * Only add new types BEFORE unknown!
 */
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
    Last // has to be last
};

enum Direction
{
    Left,
    Up,
    Down,
    Right
};
} // namespace srgsim