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
#include "log.hpp"
#include "systems.hpp"
#include "utils.hpp"

struct Config {
    int arena_max_x;
    int arena_max_y;
    int scale;
    int init_cell_count;
    int max_rounds;
    bool help;

    Config()
        : arena_max_x(50), arena_max_y(50), scale(10), init_cell_count(1500),
          max_rounds(-1), help(false) {}
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
        } else if (arg == "-m") {
            cfg.max_rounds = atoi(argv[i + 1]);
        }
    }
}

void usage(char *name) {
    std::cerr
        << name << " - Conlway's Game of Life Simulation" << std::endl
        << "-h   - Help message" << std::endl
        << "-x X - X dimension for the arena (default 50)" << std::endl
        << "-y Y - Y dimension for the arena (default 50)" << std::endl
        << "-s S - Scale at which to draw the simulation (default 10)"
        << std::endl
        << "-i I - Number of cells to start the simulation with (default 1500)"
        << std::endl
        << "-m M - Max number of rounds to run for (default fovever)"
        << std::endl;
    ;
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
    sf::Clock system_timing;
    sf::Clock round_timing;

    window.clear(sf::Color::Black);

    LOG("Starting the game of life");

    system_timing.restart();
    initialise_registry(registry, config.init_cell_count, config.arena_max_x,
                        config.arena_max_y);
    LOG("Initialise registry in " << system_timing.getElapsedTime().asSeconds()
                                  << "s");

    system_timing.restart();
    render_system(window, config.scale, registry);
    LOG("Ran render system in " << system_timing.getElapsedTime().asSeconds()
                                << "s");
    int rounds = 0;
    while (window.isOpen()) {
        round_timing.restart();
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

        system_timing.restart();
        lifecycle_system(registry);
        LOG("Ran lifecycle system in "
            << system_timing.getElapsedTime().asSeconds() << "s");

        system_timing.restart();
        render_system(window, config.scale, registry);
        LOG("Ran render system in "
            << system_timing.getElapsedTime().asSeconds() << "s");

        system_timing.restart();
        cleanup_system(registry);
        LOG("Ran cleanup system in "
            << system_timing.getElapsedTime().asSeconds() << "s");

        system_timing.restart();
        update_system(registry);
        LOG("Ran render system in "
            << system_timing.getElapsedTime().asSeconds() << "s");

        LOG("Round took " << round_timing.getElapsedTime().asSeconds() << "s");

        if (!has_alive_cells(registry)) {
            LOG("No cells left alive");
            break;
        } else if (config.max_rounds != -1 && rounds >= config.max_rounds) {
            LOG("Reached max number of rounds " << config.max_rounds);
            break;
        }
    }

    LOG("Finished the game of life");
    LOG("Ran for " << clock.getElapsedTime().asSeconds() << "s");
    LOG(rounds << " rounds");

    return EXIT_SUCCESS;
}
