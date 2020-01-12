#include <entt/entt.hpp>
#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

#include "components.hpp"
#include "utils.hpp"

namespace rc {
template <> struct Arbitrary<Position> {
    static Gen<Position> arbitrary() {
        auto range = gen::inRange(-50, 50);
        return gen::build<Position>(gen::set(&Position::x, range),
                                    gen::set(&Position::y, range));
    }
};
} // namespace rc

RC_GTEST_PROP(is_neighbour, adjacent_cells_are_neighbours,
              (const Position &pos)) {
    const auto adjacent_position =
        *rc::gen::suchThat<Position>([&](auto other) {
            return pos != other && std::abs(pos.x - other.x) <= 1 &&
                   std::abs(pos.y - other.y) <= 1;
        });
    RC_ASSERT(is_neighbour(pos, adjacent_position));
}

RC_GTEST_PROP(is_neighbour, non_adjacent_cells_are_not_neighbours,
              (const Position &pos)) {
    const auto non_adjacent_position =
        *rc::gen::suchThat<Position>([&](auto other) {
            return !(pos != other && std::abs(pos.x - other.x) <= 1 &&
                     std::abs(pos.y - other.y) <= 1);
        });
    RC_ASSERT_FALSE(is_neighbour(pos, non_adjacent_position));
}

RC_GTEST_PROP(is_neighbour, positions_are_neighbours_to_themselves,
              (const Position &pos)) {
    RC_ASSERT_FALSE(is_neighbour(pos, pos));
}

RC_GTEST_PROP(find_possible_neighbours, all_pass_is_neighbour,
              (const Position &pos)) {
    const auto neighbours = find_possible_neighbours(pos);
    RC_ASSERT(std::all_of(
        std::begin(neighbours), std::end(neighbours),
        [pos](const auto other) { return is_neighbour(pos, other); }));
}

RC_GTEST_PROP(has_alive_cells, has_some_alive_cells, ()) {
    entt::registry registry;
    auto alive_positions = *rc::gen::nonEmpty<std::unordered_set<Position>>();

    for (auto x = -50; x < 50; x++) {
        for (auto y = -50; y < 50; y++) {
            auto ent = registry.create();
            Position pos(x, y);
            registry.assign<Position>(ent, pos);
            if (std::find(std::begin(alive_positions),
                          std::end(alive_positions),
                          pos) != std::end(alive_positions)) {
                std::cout << "Creating live cells at " << pos << std::endl;
                registry.assign<entt::tag<"is_alive"_hs>>(ent);
            }
        }
    }
    RC_ASSERT(has_alive_cells(registry));
}

RC_GTEST_PROP(has_alive_cells, has_no_alive_cells, ()) {
    entt::registry registry;
    auto dim = *rc::gen::inRange<int>(1, 100);

    for (auto x = 0; x < dim; x++) {
        for (auto y = 0; y < dim; y++) {
            auto ent = registry.create();
            Position pos(x, y);
            registry.assign<Position>(ent, pos);
        }
    }
    RC_ASSERT_FALSE(has_alive_cells(registry));
}
