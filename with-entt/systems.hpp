#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

void initialise_registry(entt::registry &registry, int n_alive_cells,
                         int arena_x_max, int arena_y_max);
void lifecycle_system(entt::registry &registry);
void render_system(sf::RenderWindow &window, int scale,
                   entt::registry &registry);
void cleanup_system(entt::registry &registry);
void update_system(entt::registry &registry);
