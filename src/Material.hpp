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
        float opacity;
        float reflectivity;
    public:
        Material();
        Material(Data data);
};

#endif