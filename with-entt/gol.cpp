#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

#include "config.hpp"
#include "gol.hpp"
#include "log.hpp"


/**
 * Check if two positions are neighbours.
 */
bool is_neighbour(Position from, Position to) {
    auto dist = abs(from.x - from.y) + abs(to.x - to.y);
    return dist == 1;
}

NextCellState next_cell_state(entt::registry &registry, entt::entity entity) {
    auto view = registry.view<Position, Status>();
    auto [my_pos, my_status] = registry.get<Position, Status>(entity);
    auto neighbour_count = 0;
    for (auto entity : view) {
        auto [their_pos, their_status] = registry.get<Position, Status>(entity);
        neighbour_count += their_status.alive && is_neighbour(my_pos, their_pos);
    }

    if (my_status.alive) {
        if (neighbour_count < 2 || neighbour_count > 3) {
            return Kill;
        }
        return RemainAlive;
    }

    if (neighbour_count == 3) {
        return Resurect;
    }
    return RemainDead;
}

/**
 * Check if there are cells alive.
 */
bool has_alive_cells(entt::registry &registry) {
    auto view = registry.view<Status>();
    for (auto cell : view) {
        auto status = view.get<Status>(cell);
        if (status.alive) {
            return true;
        }
    }
    return false;
}

/**
 * Enter a cell for every available square but only make INIT_CELL_COUNT alive.
 */
void initialise_registry(entt::registry &registry) {
    std::random_device rand_dev;
    std::mt19937 rand_eng(rand_dev());
    std::uniform_int_distribution<> rand_dist(1, ARENA_MAX_X);
    std::vector<Position> alive_cells;

    LOG("Initialising registry with cells");

    LOG("Choosing " << INIT_CELL_COUNT << " cells to be alive");

    // Choose the cells that are going to be alive
    for (auto i = 0; i < INIT_CELL_COUNT; i++) {
        int x = rand_dist(rand_eng);
        int y = rand_dist(rand_eng);
        auto cell = registry.create();
        registry.assign<Position>(entity, x, y);
    }
}

/**
 * Run the Game of Life lifecycle.
 *
 * The rules are:
 *     - A live cell with two or three neighbours survives
 *     - A dead cell with three or more neighbours becomes a live cell
 *     - All other live cells die in the next generation
 *     - All other dead cells stay dead in the next generation
 */
void lifecycleSystem(entt::registry &registry) {
    LOG("Updating cell lifecycle");
    auto view = registry.view<Position, Status>();
    for (auto entity: view) {
      auto next_state = next_cell_state(registry, entity);
      if (next_state == Resurect) {
        registry.replace<Status>(entity, true);
      } else if (next_state == Kill) {
        registry.replace<Status>(entity, false);
      }
    }
}

/**
 * Render the current state.
 */
void renderSystem(sf::RenderWindow &window, entt::registry &registry) {
    LOG("Rendering current state");

    window.clear(sf::Color::Black);

    registry.view<Position, Status>().each([&window](auto &pos, auto &status) {
        if (status.alive) {
            sf::RectangleShape cell(sf::Vector2f(1.0f, 1.0f));
            cell.setPosition(pos.x, pos.y);
            window.draw(cell);
        }
    });

    window.display();
}
