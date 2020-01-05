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
#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include "log.hpp"

int main() {
    entt::registry registry;
    sf::VideoMode mode =
        sf::VideoMode(ARENA_MAX_X, ARENA_MAX_Y);
    sf::RenderWindow window(mode, "Game of Life");
    sf::Clock clock;

    window.clear(sf::Color::Black);

    LOG("Starting the game of life");

    LOG("Initialising registry");
    initialise_registry(registry, INIT_CELL_COUNT, ARENA_MAX_X, ARENA_MAX_Y);
    LOG("Rendering system");
    render_system(window, registry);
    int rounds = 0;
    while (window.isOpen()) {
      rounds++;
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
              break
            }
        }

        LOG("Running lifecycle system");
        lifecycle_system(registry);
        LOG("Rendering system");
        render_system(window, registry);
        LOG("Running cleanup system");
        cleanup_system(registry);
        LOG("Running update system");
        update_system(registry);

        // sf::sleep(sf::seconds(10));
        if (!has_alive_cells(registry)) {
            std::cerr << "No cells left alive" << std::endl;
            break;
        }
    }

    LOG("Finished the game of life");
    LOG("Ran for " << clock.getElapsedTime().asSeconds() << "s");
    LOG(rounds << " rounds");

    return EXIT_SUCCESS;
}
