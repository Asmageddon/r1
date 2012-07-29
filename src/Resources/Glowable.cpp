#include "Glowable.hpp"

#include <SFML/Graphics.hpp>

#include "../Data.hpp"

#include "../utils.hpp"

Glowable::Glowable(Data data) {
    glow_color = make_color(data["appearance"]["glow.color"]);
    glow_radius = data["appearance"]["glow.radius"].as_int();
    if (glow_color == sf::Color(0,0,0))
        glow_radius = 0;
}
