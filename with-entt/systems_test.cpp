#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_set>

#include <doctest.h>
#include <entt/entt.hpp>

#include "components.hpp"
#include "systems.hpp"
#include "utils.hpp"

#include "log.hpp"

void set_neighbours(entt::registry &reg, int n_neighbours) {}

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

TEST_SUITE("initialise_registry") {
    TEST_CASE(
        "the registry is initialise with the number of given live cells") {
        entt::registry registry;
        int n_alive_cells = 10;
        int arena_x_max = 10;
        int arena_y_max = 10;

        CAPTURE(n_alive_cells);
        CAPTURE(arena_x_max);
        CAPTURE(arena_y_max);

        initialise_registry(registry, n_alive_cells, 10, 10);

        auto view = registry.view<entt::tag<"is_alive"_hs>>();
        int count = 0;
        for (auto entity : view) {
            count++;
        }
        REQUIRE(count == n_alive_cells);
    }

    TEST_CASE("the all positions in the registry are with the given bounds") {
        entt::registry registry;

        int n_alive_cells = 10;
        int arena_x_max = 10;
        int arena_y_max = 10;

        CAPTURE(n_alive_cells);
        CAPTURE(arena_x_max);
        CAPTURE(arena_y_max);

        initialise_registry(registry, n_alive_cells, 10, 10);

        registry.view<Position>().each([arena_x_max, arena_y_max](auto &pos) {
            CHECK(pos.x >= 0);
            CHECK(pos.x <= arena_x_max);
            CHECK(pos.y >= 0);
            CHECK(pos.y <= arena_y_max);
        });
    }

    TEST_CASE(
        "the alive cells in the registry are randomly assigned positions") {
        entt::registry registry_0, registry_1;
        int n_alive_cells = 10;
        int arena_x_max = 10;
        int arena_y_max = 10;

        CAPTURE(n_alive_cells);
        CAPTURE(arena_x_max);
        CAPTURE(arena_y_max);

        initialise_registry(registry_0, n_alive_cells, 10, 10);
        initialise_registry(registry_1, n_alive_cells, 10, 10);

        std::unordered_set<Position> alive_reg_0, alive_reg_1;
        auto view = registry_0.view<entt::tag<"is_alive"_hs>>();
        for (auto entity : view) {
            auto pos = registry_0.get<Position>(entity);
            alive_reg_0.insert(pos);
        }

        view = registry_1.view<entt::tag<"is_alive"_hs>>();
        for (auto entity : view) {
            auto pos = registry_1.get<Position>(entity);
            alive_reg_1.insert(pos);
        }

        REQUIRE_FALSE(alive_reg_0.empty());
        REQUIRE_FALSE(alive_reg_1.empty());
        REQUIRE_FALSE(alive_reg_0 == alive_reg_1);
        REQUIRE(alive_reg_0.size() == alive_reg_1.size());
    }
}

TEST_SUITE("lifecycle_system") {
    TEST_CASE(
        "alive cells with two or three neighbours are tagged is_alive_next") {
        entt::registry registry;
        entt::entity cell;
        auto cell_pos = Position(2, 2);
        int x_dim = 5, y_dim = 5;
        int neighbour_count;
        std::vector<Position> possible_neighbours = {
            Position(1, 1), Position(1, 2), Position(1, 3), Position(2, 1),
            Position(2, 3), Position(3, 1), Position(3, 2), Position(3, 3)};
        std::unordered_set<Position> neighbours;

        SUBCASE("2 neighbours") {
            neighbour_count = 2;
            registry = entt::registry();
        }
        SUBCASE("3 neighbours") {
            neighbour_count = 3;
            registry = entt::registry();
        }

        neighbours.clear();

        for (auto i = 0; i < neighbour_count; i++) {
            auto pos = rand_choice(possible_neighbours.begin(),
                                   possible_neighbours.end());
            neighbours.insert(*pos);
            possible_neighbours.erase(pos);
        }

        for (auto x = 0; x < x_dim; x++) {
            for (auto y = 0; y < y_dim; y++) {
                auto pos = Position(x, y);
                auto ent = registry.create();
                registry.assign<Position>(ent, pos);
                if (pos == cell_pos ||
                    neighbours.find(pos) != neighbours.end()) {
                    registry.assign<entt::tag<"is_alive"_hs>>(ent);
                }
                if (pos == cell_pos) {
                    cell = ent;
                }
            }
        }

        CAPTURE(cell_pos);
        CAPTURE(x_dim);
        CAPTURE(y_dim);
        CAPTURE(neighbour_count);
        CAPTURE(neighbours);

        lifecycle_system(registry);

        REQUIRE(registry.has<entt::tag<"is_alive"_hs>>(cell));
        REQUIRE(registry.has<entt::tag<"is_alive_next"_hs>>(cell));
    }

    TEST_CASE("alive cells without two or three neighbours are not tagged "
              "is_alive_next") {

        entt::registry registry;
        entt::entity cell;
        auto cell_pos = Position(2, 2);
        int x_dim = 5, y_dim = 5;
        int neighbour_count;
        std::vector<Position> possible_neighbours = {
            Position(1, 1), Position(1, 2), Position(1, 3), Position(2, 1),
            Position(2, 3), Position(3, 1), Position(3, 2), Position(3, 3)};
        std::unordered_set<Position> neighbours;

        SUBCASE("1 neighbours") {
            neighbour_count = 1;
            registry = entt::registry();
        }
        SUBCASE("4 neighbours") {
            neighbour_count = 4;
            registry = entt::registry();
        }
        SUBCASE("5 neighbours") {
            neighbour_count = 5;
            registry = entt::registry();
        }
        SUBCASE("6 neighbours") {
            neighbour_count = 6;
            registry = entt::registry();
        }
        SUBCASE("7 neighbours") {
            neighbour_count = 7;
            registry = entt::registry();
        }
        SUBCASE("8 neighbours") {
            neighbour_count = 8;
            registry = entt::registry();
        }

        neighbours.clear();

        for (auto i = 0; i < neighbour_count; i++) {
            auto pos = rand_choice(possible_neighbours.begin(),
                                   possible_neighbours.end());
            neighbours.insert(*pos);
            possible_neighbours.erase(pos);
        }

        for (auto x = 0; x < x_dim; x++) {
            for (auto y = 0; y < y_dim; y++) {
                auto pos = Position(x, y);
                auto ent = registry.create();
                registry.assign<Position>(ent, pos);
                if (pos == cell_pos ||
                    neighbours.find(pos) != neighbours.end()) {
                    registry.assign<entt::tag<"is_alive"_hs>>(ent);
                }
                if (pos == cell_pos) {
                    cell = ent;
                }
            }
        }

        CAPTURE(cell_pos);
        CAPTURE(x_dim);
        CAPTURE(y_dim);
        CAPTURE(neighbour_count);
        CAPTURE(neighbours);

        lifecycle_system(registry);

        REQUIRE(registry.has<entt::tag<"is_alive"_hs>>(cell));
        REQUIRE(!registry.has<entt::tag<"is_alive_next"_hs>>(cell));
    }

    TEST_CASE(
        "dead cells with exactly three neighbours are tagged is_alive_next") {
        entt::registry registry;
        entt::entity cell;
        auto neighbour_count = 3;
        auto cell_pos = Position(2, 2);
        int x_dim = 5, y_dim = 5;
        std::vector<Position> possible_neighbours =
            find_possible_neighbours(cell_pos);
        std::unordered_set<Position> neighbours;

        neighbours.clear();

        for (auto i = 0; i < neighbour_count; i++) {
            auto pos = rand_choice(possible_neighbours.begin(),
                                   possible_neighbours.end());
            neighbours.insert(*pos);
            possible_neighbours.erase(pos);
        }

        for (auto x = 0; x < x_dim; x++) {
            for (auto y = 0; y < y_dim; y++) {
                auto pos = Position(x, y);
                auto ent = registry.create();
                registry.assign<Position>(ent, pos);
                if (neighbours.find(pos) != neighbours.end()) {
                    registry.assign<entt::tag<"is_alive"_hs>>(ent);
                }
                if (pos == cell_pos) {
                    cell = ent;
                }
            }
        }

        CAPTURE(cell_pos);
        CAPTURE(x_dim);
        CAPTURE(y_dim);
        CAPTURE(neighbour_count);
        CAPTURE(neighbours);

        lifecycle_system(registry);

        REQUIRE(!registry.has<entt::tag<"is_alive"_hs>>(cell));
        REQUIRE(registry.has<entt::tag<"is_alive_next"_hs>>(cell));
    }

    TEST_CASE("dead cells without exactly three neighbours are not tagged "
              "is_alive_next") {
        entt::registry registry;
        entt::entity cell;
        int neighbour_count;
        auto cell_pos = Position(2, 2);
        int x_dim = 5, y_dim = 5;
        std::vector<Position> possible_neighbours = {
            Position(1, 1), Position(1, 2), Position(1, 3), Position(2, 1),
            Position(2, 3), Position(3, 1), Position(3, 2), Position(3, 3)};
        std::unordered_set<Position> neighbours;

        SUBCASE("1 neighbours") { neighbour_count = 1; }
        SUBCASE("2 neighbours") { neighbour_count = 2; }
        SUBCASE("4 neighbours") { neighbour_count = 4; }
        SUBCASE("5 neighbours") { neighbour_count = 5; }
        SUBCASE("6 neighbours") { neighbour_count = 6; }
        SUBCASE("7 neighbours") { neighbour_count = 7; }
        SUBCASE("8 neighbours") { neighbour_count = 8; }

        neighbours.clear();

        for (auto i = 0; i < neighbour_count; i++) {
            auto pos = rand_choice(possible_neighbours.begin(),
                                   possible_neighbours.end());
            neighbours.insert(*pos);
            possible_neighbours.erase(pos);
        }

        for (auto x = 0; x < x_dim; x++) {
            for (auto y = 0; y < y_dim; y++) {
                auto pos = Position(x, y);
                auto ent = registry.create();
                registry.assign<Position>(ent, pos);
                if (neighbours.find(pos) != neighbours.end()) {
                    registry.assign<entt::tag<"is_alive"_hs>>(ent);
                }
                if (pos == cell_pos) {
                    cell = ent;
                }
            }
        }

        CAPTURE(cell_pos);
        CAPTURE(x_dim);
        CAPTURE(y_dim);
        CAPTURE(neighbour_count);
        CAPTURE(neighbours);

        lifecycle_system(registry);

        REQUIRE(!registry.has<entt::tag<"is_alive"_hs>>(cell));
        REQUIRE(!registry.has<entt::tag<"is_alive_next"_hs>>(cell));
    }
}

TEST_SUITE("cleanup_system") {
    TEST_CASE("entities wihout is_alive_next tag are removed") {
        entt::registry registry;

        std::random_device rand_dev;
        std::mt19937 rand_gen(rand_dev());
        std::uniform_int_distribution<> dist(0, 10);

        std::unordered_set<Position> positions = {
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
            Position(dist(rand_gen), dist(rand_gen)),
        };

        std::unordered_set<entt::entity> alive_next_round;
        std::unordered_set<entt::entity> not_alive_next_round;

        auto it = positions.begin();
        auto i = 0;

        for (; it != positions.end(); it++, i++) {
            auto entity = registry.create();
            registry.assign<Position>(entity, *it);
            if (i % 2 == 0) {
                registry.assign<entt::tag<"is_alive_next"_hs>>(entity);
                alive_next_round.insert(entity);
            } else {
                not_alive_next_round.insert(entity);
            }
        }

        for (auto entity : alive_next_round) {
            CHECK(registry.has<entt::tag<"is_alive_next"_hs>>(entity));
        }

        for (auto entity : not_alive_next_round) {
            CHECK(!registry.has<entt::tag<"is_alive_next"_hs>>(entity));
        }
    }
}

TEST_SUITE("update_system") {
    TEST_CASE(
        "add the is_alive tag to cells tagged with the is_alive_next_tag") {}

    TEST_CASE("remove the is_alive_next tag for all cells") {
        entt::registry registry;

        std::random_device rand_dev;
        std::mt19937 rand_gen(rand_dev());
        std::uniform_int_distribution<> dist(0, 10);

        std::unordered_set<entt::entity> is_alive_nexts;

        for (auto i = 0; i < 10; i++) {
            auto x = dist(rand_gen), y = dist(rand_gen);
            auto entity = registry.create();
            registry.assign<Position>(entity, x, y);
            if (i % 2 == 0) {
                registry.assign<entt::tag<"is_alive_next"_hs>>(entity);
                is_alive_nexts.insert(entity);
            }
        }

        update_system(registry);

        auto not_has_is_alive_next = [&](auto entity) {
            return !registry.has<entt::tag<"is_alive_next"_hs>>(entity);
        };

        auto has_is_alive = [&](auto entity) {
            return registry.has<entt::tag<"is_alive"_hs>>(entity);
        };

        REQUIRE(std::all_of(is_alive_nexts.begin(), is_alive_nexts.end(),
                            not_has_is_alive_next));
        REQUIRE(std::all_of(is_alive_nexts.begin(), is_alive_nexts.end(),
                            has_is_alive));
    }
}


TEST_SUITE("render_system" * doctest::skip()) {}
