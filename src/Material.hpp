#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>

#include <SFML/Graphics.hpp>

#include "Object.hpp"
#include "Glowable.hpp"
#include "Data.hpp"

class Material : public Object, public Glowable {
    public:
        std::string type;
        sf::Color color;
    public:
        Material() { /* WARNING: This does not initialize anything */};
        Material(Data data);
};

#endif