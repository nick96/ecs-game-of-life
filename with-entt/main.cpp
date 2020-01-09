#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

#include "components.hpp"
#include "config.hpp"
#include "log.hpp"
#include "systems.hpp"
#include "utils.hpp"

struct Config {
    int arena_max_x;
    int arena_max_y;
    int scale;
    int init_cell_count;
    bool help;

    Config()
        : arena_max_x(50), arena_max_y(50), scale(10), init_cell_count(1500),
          help(false) {}
};

void parse_args(int argc, char *argv[], Config &cfg) {
    for (auto i = 0; i < argc; i += 2) {
        std::string arg(argv[i]);
        if (arg == "-h") {
            cfg.help = true;
            return;
        } else if (arg == "-x") {
            cfg.arena_max_x = atoi(argv[i + 1]);
        } else if (arg == "-y") {
            cfg.arena_max_y = atoi(argv[i + 1]);
        } else if (arg == "-s") {
            cfg.scale = atoi(argv[i + 1]);
        } else if (arg == "-i") {
            cfg.init_cell_count = atoi(argv[i + 1]);
        }
    }
}

void usage(char *name) {
    std::cout
        << name << " - Conlway's Game of Life Simulation" << std::endl
        << "-h   - Help message" << std::endl
        << "-x X - X dimension for the arena (default 50)" << std::endl
        << "-y Y - Y dimension for the arena (default 50)" << std::endl
        << "-s S - Scale at which to draw the simulation (default 10)"
        << std::endl
        << "-i I - Number of cells to start the simulation with (default 1500)"
        << std::endl;
}

int main(int argc, char *argv[]) {
    Config config;
    parse_args(argc - 1, argv + 1, config);
    if (config.help) {
        usage(argv[0]);
        std::exit(1);
    }

    entt::registry registry;
    sf::VideoMode mode = sf::VideoMode(config.arena_max_x, config.arena_max_y);
    sf::RenderWindow window(mode, "Game of Life");
    sf::Clock clock;

    window.clear(sf::Color::Black);

    LOG("Starting the game of life");

    LOG("Initialising registry");
    initialise_registry(registry, config.init_cell_count, config.arena_max_x,
                        config.arena_max_y);
    LOG("Rendering system");
    render_system(window, config.scale, registry);
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
                break;
            }
        }

        LOG("Running lifecycle system");
        lifecycle_system(registry);
        LOG("Rendering system");
        render_system(window, config.scale, registry);
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
