#include <functional>

#include "components.hpp"

bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Position &lhs, const Position &rhs) {
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const Position &pos) {
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}
