#include "Displayable.hpp"

#include "../Data.hpp"

Displayable::Displayable(Data data) {
    tileset = data["appearance"]["tileset"];
    image = data["appearance"]["image"].as_int();
    variants = data["appearance"]["variants"].as_int_vector();

    blocks_sight = data["appearance"]["blocks_sight"].as_bool();

    border = data["appearance"]["border"];
}
