#include <random>
#include <unordered_set>

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "config.hpp"
#include "components.hpp"
#include "utils.hpp"


/**
 * Enter a cell for every available square but only make INIT_CELL_COUNT alive.
 */
void initialise_registry(entt::registry &registry) {
    std::random_device rand_dev;
    std::mt19937 rand_eng(rand_dev());
    std::uniform_int_distribution<> rand_dist(1, ARENA_MAX_X);
    std::unordered_set<Position> alive_cells;

    // Choose the cells that are going to be alive
    for (auto i = 0; i < INIT_CELL_COUNT; i++) {
        int x, y;
        do {
            x = rand_dist(rand_eng);
            y = rand_dist(rand_eng);
        } while (alive_cells.find(Position(x, y)) != alive_cells.end());
        alive_cells.emplace(x, y);
    }

    for (auto x = 0; x < ARENA_MAX_X; x++) {
        for (auto y = 0; y < ARENA_MAX_Y; y++) {
            auto entity = registry.create();
            registry.assign<Position>(entity, x, y);

            if (alive_cells.find(Position(x, y)) != alive_cells.end()) {
                registry.assign<entt::tag<"is_alive"_hs>>(entity);
            }
        }
    }
}


/**
 * Run the Game of Life lifecycle.
 */
void lifecycle_system(entt::registry &registry) {
    std::unordered_set<Position> alive_cells;

    // Find and set currently alive cells that will be alive in the next round
    registry.view<Position, entt::tag<"is_alive"_hs>>().each(
        [&, alive_cells](auto entity, auto &pos, auto _) mutable {
            int neighbour_count = 0;
            auto view = registry.view<Position, entt::tag<"is_alive"_hs>>();
            for (auto entity : view) {
                auto their_pos = registry.get<Position>(entity);
                if (pos != their_pos && is_neighbour(pos, their_pos)) {
                    neighbour_count += 1;
                }
            }
            if (neighbour_count == 2 || neighbour_count == 3) {
                registry.assign<entt::tag<"is_alive_next"_hs>>(entity);
            }
            alive_cells.emplace(pos.x, pos.y);
        });

    // Determine and create cells that will be alive next round but currently
    // don't exist. This uses the position map built up whilst setting the
    // currently alive cells that will be alive next round.
    registry.view<Position, entt::tag<"is_alive"_hs>>().each(
        [&, alive_cells](auto entity, auto &pos, auto _) {
            int neighbour_count = 0;
            auto possible_neighbours = find_possible_neighbours(pos);
            for (auto possible_neighbour : possible_neighbours) {
                neighbour_count +=
                    alive_cells.find(possible_neighbour) != alive_cells.end();
            }
            if (neighbour_count == 3 &&
                !registry.has<entt::tag<"is_alive"_hs>>(entity)) {
                registry.assign<entt::tag<"is_alive_next"_hs>>(entity);
            }
        });
}

/**
 * Render the current state.
 */
void render_system(sf::RenderWindow &window, entt::registry &registry) {
    window.clear(sf::Color::Black);

    registry.group<Position>(entt::get<entt::tag<"is_alive"_hs>>)
        .each([&window](auto &pos, auto _) {
                sf::RectangleShape cell(sf::Vector2f(1.0f * (SCALE - 0.5f), 1.0f * (SCALE - 0.5f)));
            cell.setPosition(pos.x * SCALE, pos.y * SCALE);
            window.draw(cell);
        });

    window.display();
}

/**
 * Remove entities that are not alive in the next round.
 */
void cleanup_system(entt::registry &registry) {
    registry
        .group<entt::tag<"is_alive"_hs>>(
            entt::exclude<entt::tag<"is_alive_next"_hs>>)
        .each([&](auto entity, auto _) { registry.destroy(entity); });
}

/**
 * Ensure entities that have the is_alive_next tag, now have the is_alive tag
 * and the is_alive_next is removed.
 */
void update_system(entt::registry &registry) {
    registry.view<entt::tag<"is_alive_next"_hs>>().each(
        [&](auto entity, auto _) {
            registry.assign_or_replace<entt::tag<"is_alive"_hs>>(entity);
            registry.remove<entt::tag<"is_alive_next"_hs>>(entity);
        });
}

