#pragma once

namespace srgsim
{

enum Type
{
    Dirt, //0
    Gras, //1
    Wall,//2
    DoorOpen,//3
    Black,//4
    Unknown,//5
    Floor,//6
    DoorClosed,//7
    Robot,//8
    White,//9
    Default,//10
    White2,//11
    CupRed,//12
    CupBlue,//13
    CupYellow,//14
    Door,//15
    Last // has to be last for iterating with ints over this enum //16
};

enum State
{
    Open,
    Closed,
    Undefined
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