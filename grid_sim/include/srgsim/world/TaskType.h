#pragma once

#include <iosfwd>

namespace srgsim
{
enum class TaskType
{
    Move,
    PickUp,
    PutDown,
    Open,
    Close,
    Idle
};

std::ostream& operator<<(std::ostream& os, const TaskType & taskType);
} // namespace srgsim
