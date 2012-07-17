#include "UnitType.hpp"

#include <string>
#include <vector>

#include "Object.hpp"
#include "Displayable.hpp"

#include "TileSprite.hpp"

#include "ResourceManager.hpp"

UnitType::UnitType(ResourceManager *resman, Data data) : Object(data), Displayable(data) {
    this->resman = resman;

    categories = data.as_str_vector("", "categories");

    material = data.as_string("body", "material");

    sprite = resman->GetSprite(tileset, image);

    sight_radius = data.as_int("stats", "sight.radius");
    sight_threshold = data.as_float("stats", "sight.threshold");

    vision_tint = data.as_Color("stats", "sight.vision_tint");

    std::cout << " * Loaded unit type: " << id << std::endl;
}