#pragma once

/**
 * Only add new types BEFORE unknown!
 */
enum Type{
    Empty, // necessary to correct ids with regard to tmx file
    Default,
    Gras,
    Wall,
    Door,
    Unknown,
    Floor,
    Robot,
    White,
    Dirt,
    Last// has to be last
};