#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <string>

#include <SFML/Graphics.hpp>

#include "Object.hpp"
#include "Data.hpp"

class Material : public Object {
    public:
        std::string type;
        sf::Color color;
        sf::Color glow_color;
        int glow_radius;
    public:
        Material();
        Material(Data data);
};

#endif