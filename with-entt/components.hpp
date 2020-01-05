#pragma once

#include <vector>

#include <entt/entt.hpp>

struct Position {
    int x;
    int y;

    Position() {}
    Position(int x_, int y_) : x(x_), y(y_) {}
};

typedef std::vector<entt::entity> Neighbours;

bool operator==(const Position &lhs, const Position &rhs);
bool operator!=(const Position &lhs, const Position &rhs);
std::ostream &operator<<(std::ostream &os, const Position &pos);
bool operator<(const Position &lhs, const Position &rhs);

namespace std {
template <> struct hash<Position> {
    inline size_t operator()(const Position &pos) const {
        std::hash<int> int_hasher;
        return int_hasher(pos.x) ^ int_hasher(pos.y);
    }
};
} // namespace std
