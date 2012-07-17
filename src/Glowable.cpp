#include "Glowable.hpp"

#include <SFML/Graphics.hpp>

#include "Data.hpp"

Glowable::Glowable(Data data) {
    glow_color = data.as_Color("appearance", "glow.color");
    glow_radius = data.as_int("appearance", "glow.radius");
}