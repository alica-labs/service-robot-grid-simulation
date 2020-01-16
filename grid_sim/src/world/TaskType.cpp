#include "srgsim/world/TaskType.h"

#include <iostream>

namespace srgsim
{
/**
 * For getting a string representation of a type of a task.
 * @param os Outputstream
 * @param type The type of a task.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const TaskType& taskType)
{
    switch (taskType) {
    case TaskType::Open:
        os << "Open";
        break;
    case TaskType::Move:
        os << "Move";
        break;
    case TaskType::Idle:
        os << "Idle";
        break;
    case TaskType::PutDown:
        os << "PutDown";
        break;
    case TaskType::PickUp:
        os << "PickUp";
        break;
    case TaskType::Close:
        os << "Close";
        break;
    case TaskType::Transport:
        os << "Transport";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace srgsim