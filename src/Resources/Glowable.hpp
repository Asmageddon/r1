#ifndef GLOWABLE_HPP_
#define GLOWABLE_HPP_

#include <SFML/Graphics.hpp>

class Data;

class Glowable {
    public:
        sf::Color glow_color;
        int glow_radius;
    public:
        Glowable() {};
        Glowable(Data data);
};

#endif
