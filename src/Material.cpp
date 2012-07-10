#include "Material.hpp"

Material::Material() {
    id = "void";
    name = "void";
    desc = "";
    type = "void";
    color = sf::Color(255,0,255);
    opacity = 1.0f;
    reflectivity = 0.0f;
    std::cout << " * Loaded material: void" << std::endl;
}
Material::Material(Data data) : Object(data) {
    type = data[""]["type"];

    color = data.as_Color("appearance", "color");

    opacity = 0.01f * data.as_int("appearance", "opacity");
    reflectivity = 0.01f * data.as_int("appearance", "reflectivity");

    std::cout << " * Loaded material: " << id << std::endl;
}