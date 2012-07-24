#include "Material.hpp"

#include "Resource.hpp"
#include "Glowable.hpp"

Material::Material(Data data) : Resource(data), Glowable(data) {
    type = data.as_string("", "type");

    color = data.as_Color("appearance", "color");

    glow_color = data.as_Color("appearance", "glow.color");
    glow_radius = data.as_int("appearance", "glow.radius");
}
