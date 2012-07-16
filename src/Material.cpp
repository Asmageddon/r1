#include "Material.hpp"

Material::Material(Data data) : Object(data) {
    type = data.as_string("", "type");

    color = data.as_Color("appearance", "color");

    glow_color = data.as_Color("appearance", "glow.color");
    glow_radius = data.as_int("appearance", "glow.radius");

    std::cout << " * Loaded material: " << id << std::endl;
}