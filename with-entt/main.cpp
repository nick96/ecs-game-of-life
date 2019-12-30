#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/System/Clock.hpp>
#include <entt/entt.hpp>

#define LOG(msg)                                                               \
    do {                                                                       \
        std::cerr << __FILE__ << ":" << __LINE__ << ": " << msg << std::endl;  \
    } while (0)

#define ARENA_MAX_X (50)
#define ARENA_MAX_Y (50)
#define SCALE (10)
#define INIT_CELL_COUNT (1000)

struct Position {
    int x;
    int y;
};

bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator!=(const Position &lhs, const Position &rhs) {
    return !(lhs == rhs);
}

struct Status {
    bool alive;
};

typedef std::vector<entt::entity> Neighbours;

/**
 * Check if two positions are neighbours.
 */
bool is_neighbour(Position from, Position to) {
    auto dist = abs(from.x - from.y) + abs(to.x - to.y);
    return dist == 1;
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
        auto pos = Position{x, y};
        alive_cells.push_back(pos);
    }

    std::sort(alive_cells.begin(), alive_cells.end(), [](auto lhs, auto rhs) {
        return lhs.x < rhs.x || lhs.y < rhs.y;
    });
    std::cerr << "Alive cells: ";
    auto prefix = "";
    for (auto alive_cell : alive_cells) {
        std::cerr << prefix << "(" << alive_cell.x << ", " << alive_cell.y
                  << ")";
        prefix = ", ";
    }
    std::cerr << std::endl;

    sf::sleep(sf::seconds(10));

    LOG("Creating cell entities and assigning their position and status");

    // Create all the entities, assigning alive status if chosen above
    for (auto x = 0; x < ARENA_MAX_X; x++) {
        for (auto y = 0; y < ARENA_MAX_Y; y++) {
            auto entity = registry.create();
            bool alive = std::find(alive_cells.begin(), alive_cells.end(),
                                   Position{x, y}) != alive_cells.end();
            registry.assign<Status>(entity, alive);
            registry.assign<Position>(entity, x, y);
            registry.assign<Neighbours>(entity, Neighbours());
        }
    }
}

/**
 * Ensure the neighbours component is up to date for all cells.
 */
void neighboursSystem(entt::registry &registry) {
    LOG("Updating neighbour components");
    registry.view<Position, Status>().each(
        [&registry](auto entity, auto &my_pos, auto &status) {
            std::vector<entt::entity> neighbours;
            if (status.alive) {
                auto neighbour_view = registry.view<Position, Status>();
                for (auto their_entity : neighbour_view) {
                    auto [their_pos, their_status] =
                        registry.get<Position, Status>(entity);
                    if (their_status.alive && is_neighbour(my_pos, their_pos)) {
                        neighbours.push_back(their_entity);
                    }
                }
            }

            registry.replace<Neighbours>(entity, neighbours);
        });
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
    registry.view<Position, Neighbours, Status>().each(
        [&registry](auto entity, auto &pos, auto &neighbours, auto &status) {
            std::remove_if(neighbours.begin(), neighbours.end(),
                           [&registry](auto neighbour) {
                               auto status = registry.get<Status>(neighbour);
                               return !status.alive;
                           });

            auto neighbour_count = neighbours.size();
            if (status.alive) {
                if (neighbour_count < 2 || neighbour_count > 3) {
                    registry.replace<Status>(entity, false);
                } else {
                    LOG("Live cell at (" << pos.x << ", " << pos.y << ")");
                }
            } else {
                if (neighbour_count == 3) {
                    LOG("Live cell at (" << pos.x << ", " << pos.y << ")");
                    registry.replace<Status>(entity, true);
                }
            }
        });
}

/**
 * Render the current state.
 */
void renderSystem(sf::RenderWindow &window, entt::registry &registry) {
    LOG("Rendering current state");

    window.clear(sf::Color::Black);

    registry.view<Position, Status>().each([&window](auto &pos, auto &status) {
        if (status.alive) {
            sf::RectangleShape cell(sf::Vector2f(SCALE / 2.0f, SCALE / 2.0f));
            cell.setPosition(pos.x * SCALE, pos.y * SCALE);
            window.draw(cell);
        }
    });

    window.display();
}

int main() {
    entt::registry registry;
    sf::VideoMode mode =
        sf::VideoMode(ARENA_MAX_X * SCALE, ARENA_MAX_Y * SCALE);
    sf::RenderWindow window(mode, "Game of Life");
    sf::Clock clock;

    window.clear(sf::Color::Black);

    LOG("Starting the game of life");

    initialise_registry(registry);
    renderSystem(window, registry);
    sf::sleep(sf::seconds(10));
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            switch (e.type) {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }

        neighboursSystem(registry);
        lifecycleSystem(registry);
        renderSystem(window, registry);

        if (!has_alive_cells(registry)) {
            std::cerr << "No cells left alive" << std::endl;
            break;
        }

        sf::sleep(sf::seconds(1));
    }

    LOG("Finished the game of life");
    LOG("Ran for " << clock.getElapsedTime().asSeconds() << "s");

    return EXIT_SUCCESS;
}
