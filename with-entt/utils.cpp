#include <iterator>
#include <random>
#include <algorithm>

#include <entt/entt.hpp>

#include "components.hpp"
#include "utils.hpp"

bool has_alive_cells(entt::registry &registry) {
    auto view = registry.view<entt::tag<"is_alive"_hs>>();
    for (auto entity : view) {
        return true;
    }
    return false;
}

bool is_neighbour(Position pos1, Position pos2) {
  auto neighbours = find_possible_neighbours(pos1);
  return std::find(neighbours.begin(), neighbours.end(), pos2) != neighbours.end();
}

std::vector<Position> find_possible_neighbours(Position pos) {
    return {Position{pos.x - 1, pos.y + 1}, Position{pos.x, pos.y + 1},
            Position{pos.x + 1, pos.y + 1},

            Position{pos.x - 1, pos.y},     Position{pos.x + 1, pos.y},

            Position{pos.x - 1, pos.y - 1}, Position{pos.x, pos.y - 1},
            Position{pos.x + 1, pos.y - 1}};
}
