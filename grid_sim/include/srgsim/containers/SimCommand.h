#pragma once

#include <essentials/Identifier.h>

#include <cstdint>

namespace srgsim
{
struct SimCommand
{
    enum Action {
        SPAWN, GOLEFT, GORIGHT, GOUP, GODOWN, PICKUP, PUTDOWN, OPEN, CLOSE
    };

    const essentials::Identifier* senderID;
    uint32_t x;
    uint32_t y;
    Action action;
};
} // namespace srgsim