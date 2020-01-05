#pragma once

#include <random>

#include <entt/entt.hpp>

#include "components.hpp"

bool has_alive_cells(entt::registry &registry);
bool is_neighbour(Position from, Position to);
std::vector<Position> find_possible_neighbours(Position pos);

template <typename Iter>
Iter rand_choice(Iter start, Iter end) {
    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_int_distribution<> dist(0, std::distance(start, end) - 1);
    std::advance(start, dist(rand_gen));
    return start;
}
