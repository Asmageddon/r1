#include "Displayable.hpp"

#include "../Data.hpp"

Displayable::Displayable(Data data) {
    tileset = data.as_string("appearance", "tileset");
    image = data.as_int("appearance", "image");
    variants = data.as_int_vector("appearance", "variants");

    blocks_sight = data.as_bool("appearance", "blocks_sight");

    border = data.as_string("appearance", "border");
}
