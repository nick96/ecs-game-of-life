#pragma once

#include <entt/entt.hpp>

#include "components.hpp"

bool has_alive_cells(entt::registry &registry);
bool is_neighbour(Position from, Position to);
std::vector<Position> find_possible_neighbours(Position pos);
