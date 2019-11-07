#include "srgsim/world/ObjectState.h"

#include <iostream>

namespace srgsim
{
/**
 * For getting a string representation of an object state.
 * @param os Outputstream
 * @param type State of an object.
 * @return Outputstream
 */
std::ostream& operator<<(std::ostream& os, const ObjectState& state)
{
    switch (state) {
    case ObjectState::Open:
        os << "Open";
        break;
    case ObjectState::Closed:
        os << "Closed";
        break;
    case ObjectState::Undefined:
        os << "Undefined";
        break;
    case ObjectState::Carried:
        os << "Carried";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}
} // namespace srgsim