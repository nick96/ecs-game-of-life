#include <random>
#include <unordered_set>

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "components.hpp"
#include "config.hpp"
#include "log.hpp"
#include "utils.hpp"

template <typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &in) {
    stream << "{";
    auto prefix = "";
    for (auto el : in) {
        stream << el;
    }
    stream << "}";
    return stream;
}


template <typename T>
std::ostream &operator<<(std::ostream &stream,
                         const std::unordered_set<T> &in) {
    stream << "{";
    auto prefix = "";
    for (auto el : in) {
        stream << el;
    }
    stream << "}";
    return stream;
}

/**
 * Initialise the registry with live cells.
 */
void initialise_registry(entt::registry &registry, int n_alive_cells,
                         int arena_x_max, int arena_y_max) {
    std::unordered_set<Position> positions;

    // Build up the collection of possible possitions
    for (auto x = 0; x < arena_x_max; x++) {
        for (auto y = 0; y < arena_y_max; y++) {
            positions.emplace(x, y);
        }
    }

    // Randomly select the live cell positions, without replacement
    for (auto i = 0; i < n_alive_cells; i++) {
        auto pos = rand_choice(positions.begin(), positions.end());
        auto entity = registry.create();
        registry.assign<Position>(entity, *pos);
        registry.assign<entt::tag<"is_alive"_hs>>(entity);
        positions.erase(pos);
    }

    for (auto pos : positions) {
        auto entity = registry.create();
        registry.assign<Position>(entity, pos);
    }
}

/**
 * Run the Game of Life lifecycle.
 */
void lifecycle_system(entt::registry &registry) {
    std::unordered_set<Position> alive_cells;

    // Find and set currently alive cells that will be alive in the next round
    registry.view<Position, entt::tag<"is_alive"_hs>>().each(
        [&](auto entity, auto &pos, auto _) mutable {
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
    registry.group<Position>(entt::exclude<entt::tag<"is_alive"_hs>>)
        .each([&, alive_cells](auto entity, auto &pos) {
            auto possible_neighbours = find_possible_neighbours(pos);
            int neighbour_count = std::count_if(
                possible_neighbours.begin(), possible_neighbours.end(),
                [alive_cells](auto pos) {
                    auto res = alive_cells.find(pos) != alive_cells.end();
                    if (res) {
                    }
                    return res;
                });
            if (neighbour_count == 3 &&
                !registry.has<entt::tag<"is_alive"_hs>>(entity)) {
                registry.assign<entt::tag<"is_alive_next"_hs>>(entity);
            }
        });
}

/**
 * Render the current state.
 */
void render_system(sf::RenderWindow &window, int scale,
                   entt::registry &registry) {
    window.clear(sf::Color::Black);

    registry.group<Position>(entt::get<entt::tag<"is_alive"_hs>>)
        .each([&](auto &pos, auto _) {
            sf::RectangleShape cell(
                sf::Vector2f(1.0f * (scale - 0.5f), 1.0f * (scale - 0.5f)));
            cell.setPosition(pos.x * scale, pos.y * scale);
            window.draw(cell);
        });

    window.display();
}

/**
 * Remove the is_alive tag from entities that are not alive in the next round.
 */
void cleanup_system(entt::registry &registry) {
    registry
        .group<entt::tag<"is_alive"_hs>>(
            entt::exclude<entt::tag<"is_alive_next"_hs>>)
        .each([&](auto entity, auto _) {
            registry.remove<entt::tag<"is_alive"_hs>>(entity);
        });
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
