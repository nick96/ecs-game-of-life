#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sstream>
#include <random>
#include <vector>

#include <doctest.h>

#include "components.hpp"
#include "utils.hpp"

TEST_CASE("position equality") {
    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_int_distribution<> dist(0, 10);

    int x = dist(rand_gen);
    int y = dist(rand_gen);

    Position pos1(x, y);
    Position pos2(x, y);

    CAPTURE(pos1);
    CAPTURE(pos2);

    REQUIRE(pos1 == pos2);
}

TEST_CASE("position inequality") {
  std::vector<Position> positions;

  for (auto x = 0; x < 20; x++) {
    for (auto y = 0; y < 20; y++) {
      positions.emplace_back(x, y);
    }
  }

  auto pos1 = rand_choice(positions.begin(), positions.end());
  positions.erase(pos1);
  auto pos2 = rand_choice(positions.begin(), positions.end());

  CAPTURE(pos1);
  CAPTURE(pos2);

  REQUIRE(pos1 != pos2);
}

TEST_CASE("position output stream") {
    std::random_device rand_dev;
    std::mt19937 rand_gen(rand_dev());
    std::uniform_int_distribution<> dist(0, 10);

    Position pos(dist(rand_gen), dist(rand_gen));

    std::stringstream ss_act;
    ss_act << pos;
    auto actual = ss_act.str();

    std::stringstream ss_exp;
    ss_exp << "(" << pos.x << ", " << pos.y << ")";

    auto expected = ss_exp.str();

    CAPTURE(expected);
    CAPTURE(actual);

    REQUIRE(actual == expected);
}
