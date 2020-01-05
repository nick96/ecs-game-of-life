#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <doctest.h>
#include <entt/entt.hpp>

#include "utils.hpp"

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

TEST_CASE("find_possible_neighbours") {
    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_int_distribution<> dist(0, 10);

    Position pos(dist(rand_gen), dist(rand_gen));
    auto possible_neighbours = find_possible_neighbours(pos);

    CAPTURE(pos);
    CAPTURE(possible_neighbours);

    for (auto possible_neighbour : possible_neighbours) {
        auto x_dist = std::abs(pos.x - possible_neighbour.x);
        auto y_dist = std::abs(pos.y - possible_neighbour.y);
        if (!(x_dist <= 1 && x_dist >= 0)) {
            FAIL("difference between x values of "
                 << pos << " and " << possible_neighbour << " is " << x_dist
                 << " not 1 or 0");
        }
        if (!(y_dist <= 1 && y_dist >= 0)) {
            FAIL("difference between y values of "
                 << pos << " and " << possible_neighbour << " is " << y_dist
                 << " not 1 or 0");
        }
    }
}
