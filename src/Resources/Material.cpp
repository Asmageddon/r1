#include "Material.hpp"

#include "Resource.hpp"
#include "Glowable.hpp"

#include "../utils.hpp"

Material::Material(Data data) : Resource(data), Glowable(data) {
    type = data[""]["type"];

    color = make_color(data["appearance"]["color"]);

    glow_color = make_color(data["appearance"]["glow.color"]);
    glow_radius = data["appearance"]["glow.radius"].as_int();
}
