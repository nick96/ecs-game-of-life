#ifndef GOL_H
#define GOL_H

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct Position {
    int x;
    int y;
};

struct Status {
    bool alive;
};

enum NextCellState {
    Resurect,
    Kill,
    RemainAlive,
    RemainDead,
};

typedef std::vector<entt::entity> Neighbours;

bool has_alive_cells(entt::registry &registry);
bool is_neighbour(Position from, Position to);
NextCellState next_cell_state(entt::registry &registry, entt::entity entity);

void initialise_registry(entt::registry &registry);
void lifecycleSystem(entt::registry &registry);
void renderSystem(sf::RenderWindow &window, entt::registry &registry);


inline bool operator==(const Position &lhs, const Position &rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const Position &lhs, const Position &rhs) {
    return !(lhs == rhs);
}

inline std::ostream &operator<<(std::ostream &os, const Position &pos) {
  os << "(" << pos.x << ", " << pos.y << ")";
  return os;
}

#endif
