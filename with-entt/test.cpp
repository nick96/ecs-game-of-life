#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sstream>
#include <stdexcept>

#include "gol.hpp"
#include <doctest.h>
#include <entt/entt.hpp>

TEST_CASE("is_neighbour") {
    Position from, to;
    bool are_neighbours;
    SUBCASE("North") {
        from = Position{5, 5};
        to = Position{5, 6};
        are_neighbours = true;
    }
    SUBCASE("South") {
        from = Position{5, 5};
        to = Position{5, 4};
        are_neighbours = true;
    }
    SUBCASE("East") {
        from = Position{5, 5};
        to = Position{6, 5};
        are_neighbours = true;
    }
    SUBCASE("West") {
        from = Position{5, 5};
        to = Position{4, 5};
        are_neighbours = true;
    }
    SUBCASE("North-East") {
        from = Position{5, 5};
        to = Position{5, 6};
        are_neighbours = true;
    }
    SUBCASE("North-West") {
        from = Position{5, 5};
        to = Position{5, 6};
        are_neighbours = true;
    }
    SUBCASE("South-East") {
        from = Position{5, 5};
        to = Position{5, 4};
        are_neighbours = true;
    }
    SUBCASE("South-West") {
        from = Position{5, 5};
        to = Position{5, 4};
        are_neighbours = true;
    }
    SUBCASE("East") {
        from = Position{5, 5};
        to = Position{6, 5};
        are_neighbours = true;
    }
    SUBCASE("West") {
        from = Position{5, 5};
        to = Position{4, 5};
        are_neighbours = true;
    }
    SUBCASE("Not neighbour") {
        from = Position{5, 5};
        to = Position{3, 5};
        are_neighbours = false;
    }

    CAPTURE(from);
    CAPTURE(to);
    CAPTURE(are_neighbours);

    if (are_neighbours) {
        REQUIRE(is_neighbour(from, to));
        REQUIRE(is_neighbour(to, from));
    } else {
        REQUIRE(!is_neighbour(from, to));
        REQUIRE(!is_neighbour(to, from));
    }
}

enum State { Alive, Dead, NA };

std::ostream &operator<<(std::ostream &os,
                         const std::vector<std::vector<State>> &states) {
    os << "{";

    for (std::vector<State> row : states) {
        os << "{";
        auto prefix = "";
        for (State state : row) {
            if (state == Alive) {
                os << prefix << 1;
            } else if (state == Dead) {
                os << prefix << 0;
            } else if (state == NA) {
                os << prefix << -1;
            } else {
                std::stringstream msg;
                msg << "Unknown state: " << state;
                throw std::invalid_argument(msg.str());
            }
            prefix = ", ";
        }

        os << "}";
    }

    os << "}";

    return os;
}

TEST_CASE("next_cell_state") {
    entt::registry registry;
    entt::entity cell;
    NextCellState expected;
    std::vector<std::vector<State>> neighbour_states;

    Position pos = Position{3, 3};

    SUBCASE("alive") {
        cell = registry.create();
        registry.assign<Status>(cell, true);
        registry.assign<Position>(cell, pos);
        SUBCASE("0 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, NA, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Alive, Dead, Dead, Dead, Alive},
            };
        }

        SUBCASE("1 neighbour") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Dead, Dead, Dead}, {Dead, Dead, Dead, Dead, Dead},
                {Dead, Alive, NA, Dead, Dead},  {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("2 neighbours") {
            expected = RemainAlive;
            neighbour_states = {
                {Alive, Dead, Dead, Dead, Alive},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("3 neighbours") {
            expected = RemainAlive;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("4 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
            };
        }

        SUBCASE("5 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
            };
        }

        SUBCASE("6 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }

        SUBCASE("7 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Alive, Dead, Dead, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }

        SUBCASE("8 neighbours") {
            expected = Kill;
            neighbour_states = {
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }
    }

    SUBCASE("dead") {
        cell = registry.create();
        registry.assign<Status>(cell, false);
        registry.assign<Position>(cell, pos);
        SUBCASE("0 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, NA, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Alive, Dead, Dead, Dead, Alive},
            };
        }

        SUBCASE("1 neighbour") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Dead, Dead, Dead}, {Dead, Dead, Dead, Dead, Dead},
                {Dead, Alive, NA, Dead, Dead},  {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("2 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Alive, Dead, Dead, Dead, Alive},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("3 neighbours") {
            expected = Resurect;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Dead, Dead, Dead},
                {Dead, Dead, Dead, Dead, Dead},
            };
        }

        SUBCASE("4 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
            };
        }

        SUBCASE("5 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
            };
        }

        SUBCASE("6 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Alive, Dead, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }

        SUBCASE("7 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Dead, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Alive, Dead, Dead, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }

        SUBCASE("8 neighbours") {
            expected = RemainDead;
            neighbour_states = {
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, NA, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
                {Dead, Alive, Alive, Alive, Dead},
            };
        }
    }

    CAPTURE(registry);
    CAPTURE(cell);
    CAPTURE(expected);
    CAPTURE(neighbour_states);
    CAPTURE(pos);

    for (auto x = 0; x < neighbour_states.size(); x++) {
        for (auto y = 0; y < neighbour_states[x].size(); y++) {
            if (neighbour_states[x][y] != NA) {
                auto status = neighbour_states[x][y] == Alive;
                auto new_cell = registry.create();
                registry.assign<Position>(new_cell, x, y);
                registry.assign<Status>(new_cell, status);
            }
        }
    }

    REQUIRE(next_cell_state(registry, cell) == expected);
}
