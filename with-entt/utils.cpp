#include <algorithm>
#include <iterator>
#include <random>

#include <entt/entt.hpp>

#include "components.hpp"
#include "utils.hpp"

bool has_alive_cells(entt::registry &registry) {
    auto view = registry.view<entt::tag<"is_alive"_hs>>();
    return view.size() > 0;
}

bool is_neighbour(Position pos1, Position pos2) {
    return pos1 != pos2 && std::abs(pos1.x - pos2.x) <= 1 &&
           std::abs(pos1.y - pos2.y) <= 1;
}

std::vector<Position> find_possible_neighbours(Position pos) {
    return {Position{pos.x - 1, pos.y + 1}, Position{pos.x, pos.y + 1},
            Position{pos.x + 1, pos.y + 1},

            Position{pos.x - 1, pos.y},     Position{pos.x + 1, pos.y},

            Position{pos.x - 1, pos.y - 1}, Position{pos.x, pos.y - 1},
            Position{pos.x + 1, pos.y - 1}};
}
