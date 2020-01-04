#include "utils.hpp"

#include <entt/entt.hpp>

#include "components.hpp"

bool has_alive_cells(entt::registry &registry) {
    auto view = registry.view<entt::tag<"is_alive"_hs>>();
    for (auto entity : view) {
        return true;
    }
    return false;
}

bool is_neighbour(Position pos1, Position pos2) {
    auto dist = abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
    return dist == 1;
}

std::vector<Position> find_possible_neighbours(Position pos) {
    return {Position{pos.x + 1, pos.y + 1}, Position{pos.x, pos.y + 1},
            Position{pos.x + 1, pos.y + 1},

            Position{pos.x - 1, pos.y},     Position{pos.x + 1, pos.y},

            Position{pos.x - 1, pos.y - 1}, Position{pos.x, pos.y - 1},
            Position{pos.x + 1, pos.y - 1}};
}
