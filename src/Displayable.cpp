#include "Displayable.hpp"

Displayable::Displayable(Data data) {
    tileset = data.as_string("appearance", "tileset");
    image = data.as_int("appearance", "image");
    variants = data.as_int_vector("appearance", "variants", true);

    glow_color = data.as_Color("appearance", "glow.color");
    glow_radius = data.as_int("appearance", "glow.radius");

    border = data.as_string("appearance", "border");
}