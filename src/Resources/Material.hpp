#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "../AString.hpp"

#include <SFML/Graphics.hpp>

#include "../Data.hpp"
#include "Resource.hpp"
#include "Glowable.hpp"

class Material : public Resource, public Glowable {
    public:
        AString type;
        sf::Color color;
    public:
        Material() { /* WARNING: This does not initialize anything */};
        Material(Data data);
};

#endif
