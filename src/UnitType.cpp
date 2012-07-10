#include "UnitType.hpp"

UnitType::UnitType() : Object(), Displayable() {
    id = "missingno";
    name = "MissingNo";
    desc = "";
    type = "";
    categories.push_back("invalid");
    max_hp = 10;

    tileset = "units1.png";
    image = 0;

    std::cout << " * Loaded unit type: missingno" << std::endl;
}

UnitType::UnitType(Data data) : Object(data), Displayable(data) {
    categories = data.as_str_vector("", "categories");
    max_hp = data.as_int("stats", "max_hp");
    std::cout << " * Loaded unit type: " << id << std::endl;
}