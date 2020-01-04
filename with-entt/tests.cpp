#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "systems.hpp"
#include "utils.hpp"
#include "components.hpp"

TEST_CASE("initialise_registry") {
  REQUIRE(false);
}

TEST_CASE("lifecycle_systerm") {
  REQUIRE(false);
}

TEST_CASE("render_system") {
  REQUIRE(false);
}

TEST_CASE("cleanup_system") {
  REQUIRE(false);
}

TEST_CASE("update_system") {
  REQUIRE(false);
}

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
