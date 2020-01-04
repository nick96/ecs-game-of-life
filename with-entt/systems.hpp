#pragma once

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

void initialise_registry(entt::registry &registry);
void lifecycle_system(entt::registry &registry);
void render_system(sf::RenderWindow &window, entt::registry &registry);
void cleanup_system(entt::registry &registry);
void update_system(entt::registry &registry);
