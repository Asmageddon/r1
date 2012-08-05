#include "Material.hpp"

#include "Resource.hpp"
#include "Glowable.hpp"

#include "../utils.hpp"

Material::Material(Data data) : Resource(data), Glowable(data) {
    type = data[""]["type"];

    color = make_color(data["appearance"]["color"]);

    glow_color = make_color(data["appearance"]["glow.color"]);
    glow_radius = data["appearance"]["glow.radius"].as_int();

    density = data["properties"]["density"].as_float();

    durability = data["properties"]["durability"].as_float();
    resilience = data["properties"]["resilience"].as_float();

    sharpness = data["properties"]["sharpness"].as_float();
    elasticity = data["properties"]["elasticity"].as_float();
}
