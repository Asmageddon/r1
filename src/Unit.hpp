#ifndef UNIT_HPP_
#define UNIT_HPP_

#include <SFML/Graphics.hpp>

#include "UnitType.hpp"

class Level;

class Unit {
    public:
        u_int16_t type;
        u_int16_t material;
        u_int16_t integrity;
        u_int16_t temperature;

        u_int32_t hp;

        Level *location;
        sf::Vector2i pos;

    public:
        Unit(const UnitType& prototype) { }
        Unit() {
            type = 0;
            material = 0;
            integrity = 0;
            temperature = 0;

            hp = 1;
            pos = sf::Vector2i(0,0);

        }

        void Move(const sf::Vector2i& vec);
};
#endif