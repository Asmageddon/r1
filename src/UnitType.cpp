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
    max_hp = data.as_int("stats", "max_hp");

    material = data.as_string("body", "material");

    sprite = resman->GetSprite(tileset, image);

    std::cout << " * Loaded unit type: " << id << std::endl;
}