#pragma once

namespace srgsim
{

enum Type
{
    Dirt,
    Gras,
    Wall,
    DoorOpen,
    Black,
    Unknown,
    Floor,
    DoorClosed,
    Robot,
    White,
    Default,
    White2,
    CupRed,
    CupBlue,
    CupYellow,
    Last // has to be last for iterating with ints over this enum
};

enum Direction
{
    Left,
    Up,
    Down,
    Right,
    None
};
} // namespace srgsim