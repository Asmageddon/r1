#include "Displayable.hpp"

Displayable::Displayable(Data data) {
    tileset = data.as_string("appearance", "tileset");
    image = data.as_int("appearance", "image");
    variants = data.as_int_vector("appearance", "variants", true);

    border = data.as_string("appearance", "border");
}