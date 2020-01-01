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

int main() {
    entt::registry registry;
    sf::VideoMode mode =
        sf::VideoMode(ARENA_MAX_X, ARENA_MAX_Y);
    sf::RenderWindow window(mode, "Game of Life");
    sf::Clock clock;

    window.clear(sf::Color::Black);

    LOG("Starting the game of life");

    initialise_registry(registry);
    renderSystem(window, registry);
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        lifecycleSystem(registry);
        renderSystem(window, registry);

        if (!has_alive_cells(registry)) {
            std::cerr << "No cells left alive" << std::endl;
            break;
        }
    }

    LOG("Finished the game of life");
    LOG("Ran for " << clock.getElapsedTime().asSeconds() << "s");

    return EXIT_SUCCESS;
}
